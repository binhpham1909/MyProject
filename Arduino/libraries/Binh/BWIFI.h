#ifndef BWIFI_h
#define BWIFI_h

#include <ESP8266WiFi.h>
#include "BDef.h"
#include "BData.h"

class BWIFI: public BData 
{
    public: 
        void init(); 
        uint32_t getServer(void);
        uint16_t getServerPort(void);
        uint8_t getRequestTimeout(void);
        void connect(void);
        boolean checkConnected(void);
        void reConnect(void);
        uint8_t getConnectTimeOut(void);
        void setSTAMode(void);
        void setAPMode(void);  
    private:  
        boolean CONNECTED=false;
        boolean	APMODE=false;  
        uint32_t lastConnect = 0;
        uint32_t lastCheckUnConnect = 0;
};
#endif