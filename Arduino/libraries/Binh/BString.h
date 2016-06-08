#ifndef BString_h
#define BString_h

#include <ESP8266WiFi.h>

class BString{
    public:
        uint8_t numberOfCharInString(String *StringFrom, String findChar);
        boolean StringToArray(String *StringFrom, char* arrayTo, int maxlen);
        uint32_t StringToIPAdress(String IPvalue);    
        String htmlGETValue(String *_request,String _key);
    protected:
        String StringToKeyValue(String *_request, String _separate, String _end,String _enall, String *_key);    
};
#endif