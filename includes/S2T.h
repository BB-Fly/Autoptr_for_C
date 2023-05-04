#ifndef S2T_H_GUARD
#define S2T_H_GUARD


#include<string>
#include<stdexcept>
#include"CStr.h"
#include"PtrTable.h"
#include<regex>
#include<iostream>
#include"OBuffer.h"

using namespace std;

class S2T{
private:
    PtrTable m_ptr_table;
    OBuffer m_buf;
    string m_text;
    int m_idx;

    void __beg_handle();
    string __get_token();
    int __get_ptr();
    string __pre_str(const string& s);
    string __get_type();
    string __get_name();

    void __handle_str();

    void __handle_def(int idx,const string& dtype, const string& name);
    void __handle_func(int idx,const string& dtype, const string& name);
    void __handle_new(const string& name);
    void __handle_cpy(const string& name);
    
public:
    S2T(const string& i,const string& o);
    void Run();
};


#endif