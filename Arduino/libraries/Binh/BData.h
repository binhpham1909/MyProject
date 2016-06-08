#ifndef BData_h
#define BData_h
#include "BDef.h"
#include <EEPROM.h>  // used: EEPROM.func
#include <IPAddress.h>  // used: EEPROM.func
class BData{
    public:
        void readData(void);
        void restoreDefaults(void);
    protected:
        char wf_ssid[MAX_SSID_LEN];
        char wf_password[MAX_PASSWORD_LEN];
        char wf_key[MAX_PASSWORD_LEN];
        char ap_ssid[MAX_SSID_LEN];
        char ap_password[MAX_PASSWORD_LEN];
        char dv_password[MAX_PASSWORD_LEN];
        boolean used_DHT = false;
        uint8_t DHT_type;
        uint8_t DHT_pin;
        uint32_t server_ip;
        int server_port;
        uint8_t wf_connect_timeout;
        uint8_t wf_request_timeout;
};
#endif