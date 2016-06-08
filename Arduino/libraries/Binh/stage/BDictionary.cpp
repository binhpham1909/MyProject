#include "BDictionary.h"


String BDict::get(String _key){
    uint16_t _index = checkKey(_key);
    if(_index!= NULL_KEY)
        return m_DICT[_index].value;
    else
        return "";
}
uint16_t BDict::getDictSize(){
    return d_size;
}
uint16_t BDict::checkKey(String _key){
    uint16_t _index = NULL_KEY;
    for(uint16_t i=0; _index!=NULL_KEY; i++)
        if(m_DICT[i].key==_key)
            _index = i;
    return _index;
}

void BDict::set(String _key, String _value){
    uint16_t _index = checkKey(_key);
    if(_index!= NULL_KEY)   // update value
        updateDict(_index, _value);
    else{
        Dict _kv;
        _kv.key = _key;
        _kv.value = _value;
        insertDict(_kv);         
    }    // insert value
}

uint16_t BDict::insertDict(Dict &_kv){
    d_size++;
    Dict *_DICT = new Dict[d_size];
    if(d_size > 1)
        for(uint16_t i=0; i< d_size - 1; i++)
            _DICT[i] = m_DICT[i];
    _DICT[d_size - 1] = _kv;
    delete m_DICT;
    m_DICT = _DICT;
    return d_size - 1;
};
void BDict::updateDict(uint16_t _index, String _value){
    m_DICT[_index].value = _value;
};