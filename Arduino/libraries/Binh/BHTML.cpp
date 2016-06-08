#include "BHTML.h"

void BHTML::createPOSTRequest(void){
    
};
void BHTML::createGETRequest(void){
    
};
void BHTML::createRequest(void){
    
};
// Add key, value to GETRequest String
void BHTML::addGETKeyValue(String *_s, String _key,String _val) {
    *_s += "&&" + _key + "=" + _val;
}