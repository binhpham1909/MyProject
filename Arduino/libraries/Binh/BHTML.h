#ifndef BHTML_h
#define BHTML_h

#include <ESP8266WiFi.h>
#include "BDef.h"
#include "BJSON.h"
// Request encode
#define	ONEGET	1
#define	FIRSTGET	2
#define	NEXTGET	3
#define	LASTGET	4


class BHTML{
    public:
        void createPOSTRequest(void);
        void createGETRequest(void);
    private:
        void addGETKeyValue(String *_s, String _key,String _val);
        void createRequest(void);
};
#endif