#include "OBuffer.h"
#include<string>
#include<iostream>
#include<fstream>



OBuffer::OBuffer(const string& o, int s){
    m_target_file.open(o,ios_base::out);
    m_size = s;
}

void OBuffer::write(const string& s){
    if(s.length()+m_buf.length()>m_size){
        m_target_file << m_buf;
        m_buf.clear();
        if(s.size()>m_size){
            m_target_file << s;
        }
    }else{
        m_buf += s;
    }
}

OBuffer::~OBuffer(){
    m_target_file << m_buf;
    m_target_file.close();
}