#include"S2T.h"

using namespace std;


S2T::S2T(const string& i,const string& o):m_buf(o),m_idx(0),m_ptr_table(){
    ifstream m_source_file;
    m_source_file.open(i,ios_base::in);
    m_source_file.seekg(0,ios_base::end);
    int len = m_source_file.tellg();
    m_source_file.seekg(ios_base::beg);
    char* buf = new char[len+1]();
    m_source_file.read(buf,len+1);
    m_text = string(buf,len+1);
    while (isspace(m_text.back())||m_text.back()=='\0') m_text.pop_back();
    delete[] buf;
    m_source_file.close();
}

void S2T::__beg_handle(){
    int idx = m_idx;
    if(__get_token()=="#ifndef"&&
       __get_token()=="__AUTO_PTR_DUARD__"&&
       __get_token()=="#define"&&
       __get_token()=="__AUTO_PTR_DUARD__"&&
       __get_token()=="#define"&&
       __get_token()=="auto_ptr(__type__name__)"&&
       __get_token()=="__type__name__*"&&
       __get_token()=="#define"&&
       __get_token()=="MALLOC(__type__name__,__size__)"&&
       __get_token()=="(__type__name__*)malloc(sizeof(__type__name__)*(__size__))"&&
       __get_token()=="#endif"
       );
    else m_idx = idx;
    m_buf.write(AUTO_PTR_STR::Header);
    return;
}

//idx-> token[ ]
string S2T::__get_token(){
    string res;
    while(m_idx<m_text.length()&&isspace(m_text[m_idx])) m_idx++; 
    while(m_idx<m_text.length()&&!isspace(m_text[m_idx]))
        if(m_text[m_idx]=='"') __handle_str();
        else{
            if(m_text[m_idx]=='{') m_ptr_table.layer_add();
            else if(m_text[m_idx]=='}') m_ptr_table.layer_sub();
            res = res+m_text[m_idx++];
        }
    return res;
}

string S2T::__pre_str(const string& s){
    string res;
    for(char c:s){
        if(isdigit(c)||isalpha(c)||c=='_') res+=c;
        else break;
    }
    return res;
}

//idx->[a]uto_ptr or [p]tr = ...
int S2T::__get_ptr(){
    while (true)
    {
        int t_idx = m_idx;
        string tmp = __get_token();
        tmp = __pre_str(tmp);
        if(tmp=="auto_ptr"||m_ptr_table.find(tmp)!=AUTO_PTR_STR::NoFind){
            m_idx = t_idx;
            return tmp=="auto_ptr"?1:2;
        }else if(m_idx>=m_text.length()){
            return 0;
        }
    }
}

//idx->auto_ptr(type)[ ]
string S2T::__get_type(){
    string res;
    while(m_text[m_idx]!='(') m_idx++;
    m_idx++;
    while(m_text[m_idx]!=')') if(!isspace(m_text[m_idx])) res+= m_text[m_idx++]; else m_idx++;
    m_idx++;
    return res;
}

//idx->name[ ]
string S2T::__get_name(){
    string res;
    while(isspace(m_text[m_idx])) m_idx++;
    while(isdigit(m_text[m_idx])||isalpha(m_text[m_idx])||m_text[m_idx]=='_') res+=m_text[m_idx++];
    return res;
}

void S2T::__handle_str(){
    while(m_text[m_idx]!='"') m_idx++;
    m_idx++;
}

//idx->func()[ ]
void S2T::__handle_func(int idx,const string& dtype, const string& name){
    string res = dtype + "* " + name;
    while(m_text[m_idx]!=')') res+=m_text[m_idx++];
    res += m_text[m_idx++];
    m_buf.write(res);
}

//idx->auto_ptr(type) [ ]ptr = ... 
void S2T::__handle_def(int idx,const string& dtype, const string& name){
    string res = "autoptr_def("+dtype+"*,"+name+");\n";
    m_buf.write(res);
    while(isspace(m_text[m_idx])) m_idx--;
    while(!isspace(m_text[m_idx])&&m_text[m_idx]!=')') m_idx--;
    m_ptr_table.insert(name,dtype);
}

//idx->ptr.cpy(num)[]
void S2T::__handle_cpy(const string& name){
    while(m_text[m_idx]!='(') m_idx++;
    m_idx++;
    while(isspace(m_text[m_idx])) m_idx++;
    string dgt;
    while(isdigit(m_text[m_idx])) dgt += m_text[m_idx++];
    while(m_text[m_idx]!=')') m_idx++;
    m_idx++;
    int i = stoi(dgt);
    string text = "autoptr_cpy("+name+")";
    while(i--){
        m_buf.write(text);
        if(i!=0) m_buf.write(",\n");
    }
}

//idx->ptr=MALLOC(type,num)[]
void S2T::__handle_new(const string& name){
    while(isspace(m_text[m_idx])||m_text[m_idx]=='(') m_idx++;
    string dtype = __get_name();
    while(isspace(m_text[m_idx])||m_text[m_idx]==',') m_idx++;
    string dgt = __get_name();
    while(m_text[m_idx]!=')') m_idx++;
    m_idx++;
    string res = "autoptr_new("+name+",sizeof("+dtype+")*"+dgt+")";
    m_buf.write(res);
}

void S2T::Run(){
    __beg_handle();
    while(m_idx<m_text.length()){
        int idx=m_idx;
        if(int i = __get_ptr()){
            while(isspace(m_text[m_idx])) m_idx++;
            m_buf.write(m_text.substr(idx,m_idx-idx));
            
            idx = m_idx;
            if(i==1){
                string dtype = __get_type();
                string name = __get_name();
                if(m_text[m_idx]=='(') __handle_func(idx,dtype,name);
                else __handle_def(idx,dtype,name);
            }else{
                string name = __get_name();
                if(m_text[m_idx]=='.') __handle_cpy(name);
                else{
                    while(isspace(m_text[m_idx])) m_idx++;
                    if(m_text[m_idx]=='='){
                        m_idx++;
                        if(__get_name()=="MALLOC"){
                            __handle_new(name);
                        }else{
                            m_buf.write(m_text.substr(idx,m_idx-idx));
                        }
                    }else{
                        m_buf.write(m_text.substr(idx,m_idx-idx));
                    }
                }
            }
        }else{
            m_buf.write(m_text.substr(idx,m_idx-idx));
        }
    }
}