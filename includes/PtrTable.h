#ifndef PTRTABLE_H_GUARD
#define PTRTABLE_H_GUARD

#include<string>
#include<unordered_map>
#include<unordered_set>
#include<vector>

using namespace std;

class PtrTable{
    typedef string TypeName;
    typedef string ObjectName;
    
private:
    unordered_map<ObjectName,vector<TypeName>> m_table;
    vector<unordered_set<ObjectName>> m_layer;
    int m_depth;
public:
    PtrTable():m_table(),m_layer(),m_depth(-1){}
    void insert(const ObjectName& o, const TypeName& t);
    string find(const ObjectName& o);
    void layer_add();
    void layer_sub();
};


#endif