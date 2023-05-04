#include"PtrTable.h"
#include"CStr.h"

void PtrTable::layer_add(){
    ++m_depth;
    m_layer.emplace_back(unordered_set<ObjectName>());
}

void PtrTable::layer_sub(){
    for(auto it=m_layer[m_depth].begin();it!=m_layer[m_depth].end();++it){
        m_table[*it].pop_back();
    }
    --m_depth;
    m_layer.pop_back();
}


void PtrTable::insert(const ObjectName& o, const TypeName& t){
    if(m_depth<0) layer_add();
    m_layer[m_depth].insert(o);
    m_table[o].emplace_back(t);
}

string PtrTable::find(const ObjectName& o){
    if(m_table.count(o)){
        return m_table[o].back();
    }else{
        return AUTO_PTR_STR::NoFind;
    }
}