#ifndef BJSON_h
#define BJSON_h

#include <ESP8266WiFi.h>
#include <PGMSPACE.h>
// Json encode
#define	BJSONONE	1
#define	BJSONFIRST	2
#define	BJSONNEXT	3
#define	BJSONLAST	4

class BJSON{
    public:
        void JsonEncode(String * _s, int _position, String _key, String _val);
        String JsonDecode(String * _s, String _key);
    private:
        void JsonAdd(String *_s, String _key,String _val);
        String BJSON_OPEN_BRAKE = "{\r\n";
        String BJSON_CLOSE_BRAKE = "\r\n}";
        String BJSON_NEW_LINE = ",\r\n";
};
#endif