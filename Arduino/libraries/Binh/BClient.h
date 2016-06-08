#ifndef BClient_h
#define BClient_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "BDef.h"
#include "BHTML.h"

#ifdef DEBUG_ESP_HBI
#ifdef DEBUG_ESP_PORT
#define DEBUG_HBI(...) DEBUG_ESP_PORT.print( __VA_ARGS__ )
#define DEBUGln_HBI(...) DEBUG_ESP_PORT.println( __VA_ARGS__ )


#endif
#endif

#ifndef DEBUG_HBI
#define DEBUG_HBI(...)
#define DEBUGln_HBI(...)
#endif

class BClient{
    public:
        boolean sendRequest(uint32_t _server,uint16_t port, String *_tosend, uint8_t _request_timeout);
        boolean checkRespone(void);
        void getRespone(String &_respone);
    private:
        WiFiClient client;
        uint32_t lastCheckTime =0;  // check time to calculate delta time replace delay function
        uint32_t requestTimeOut = 30000;    // time delay wait for server respone
        boolean sendSuccess = false;
        String respone = "";
};
#endif