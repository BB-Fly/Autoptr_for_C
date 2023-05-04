#ifndef OBuffer_H_GUARD
#define OBuffer_H_GUARD


#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class OBuffer{
private:
    ofstream m_target_file;
    string m_buf;
    int m_size;
public:
    OBuffer(const string& o, int s=(1<<24));
    void write(const string& s);
    ~OBuffer();
};

#endif