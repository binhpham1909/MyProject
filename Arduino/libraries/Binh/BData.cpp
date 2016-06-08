#include "BData.h"

void BData::readData(void){
    restoreDefaults();
    DEBUGln_HBI("Open EEPROM");
    EEPROM.begin(MAX_EEPROM_SIZE);
    EEPROM.get(WF_SSID_ADD,wf_ssid);
    DEBUG_HBI(FPSTR(lb_SSID));  DEBUGln_HBI(wf_ssid);
    EEPROM.get(WF_PASSWORD_ADD,wf_password);
    DEBUG_HBI(FPSTR(lb_PASSWORD));  DEBUGln_HBI(wf_password);
    EEPROM.get(WF_KEY_ADD,wf_key);
    DEBUG_HBI(FPSTR(lb_KEY));   DEBUGln_HBI(wf_key);
    EEPROM.get(AP_SSID_ADD,ap_ssid);
    DEBUG_HBI(FPSTR(lb_AP));    DEBUGln_HBI(ap_ssid);
    EEPROM.get(AP_PASSWORD_ADD,ap_password);
    DEBUG_HBI(FPSTR(lb_AP));    DEBUGln_HBI(ap_password);
    EEPROM.get(DV_PASSWORD_ADD,dv_password);
    DEBUG_HBI(FPSTR(lb_ADMIN_PASSWORD));    DEBUGln_HBI(dv_password);
    EEPROM.get(SERVER_IP_ADD,server_ip);
    DEBUG_HBI(FPSTR(lb_SERVER));    DEBUGln_HBI(server_ip);
    EEPROM.get(SERVER_PORT_ADD,server_port);
    DEBUG_HBI(FPSTR(lb_SERVER_PORT));    DEBUGln_HBI(server_port);
    used_DHT =(EEPROM.read(USED_DHT_ADD)==1)?true:false;
    DEBUG_HBI(F("Used DHT\t"));    DEBUGln_HBI(used_DHT);
    DHT_type = EEPROM.read(DHT_TYPE_ADD);
    DEBUG_HBI(F("DHT Type\t"));    DEBUGln_HBI(DHT_type);
    DHT_pin = EEPROM.read(DHT_PIN_ADD);
    DEBUG_HBI(F("DHT PIN\t"));     DEBUGln_HBI(DHT_pin);
    wf_connect_timeout  = EEPROM.read(WF_CONNECT_TIMEOUT_ADD);
    DEBUG_HBI(F("Time out connect wifi\t"));   DEBUGln_HBI(wf_connect_timeout);
    wf_request_timeout  = EEPROM.read(WF_REQUEST_TIMEOUT_ADD);
    DEBUG_HBI(F("Timeout send request\t"));    DEBUGln_HBI(wf_request_timeout);
    EEPROM.end();
    DEBUGln_HBI(F("Close EEPROM"));
};
void BData::restoreDefaults(void){
    DEBUGln_HBI(F("Open EEPROM"));
    EEPROM.begin(MAX_EEPROM_SIZE);
    if(EEPROM.read(FIRST_SETUP)!=9){
        DEBUGln_HBI(F("Restore."));
        String _df_ssid = "TempHumiSensor";
        String _ssid = "Tang_2_nha_moi";
        String _password = "0313837124";
        char _arr_32[32];
        char _arr_10[10];
        char _arr_48[48];
        char _arr_ip[4];
        _arr_ip[0] = 10;
        _arr_ip[1] = 0;
        _arr_ip[2] = 0;
        _arr_ip[3] = 79;
        int _port = 8989;
        
        _ssid.toCharArray(_arr_32,_ssid.length()+1);
        EEPROM.put(WF_SSID_ADD,_arr_32);
        _df_ssid.toCharArray(_arr_32,_df_ssid.length()+1);
        EEPROM.put(AP_SSID_ADD,_arr_32);
        _password.toCharArray(_arr_32,_password.length()+1);
        EEPROM.put(WF_PASSWORD_ADD,_arr_32);
        EEPROM.put(AP_PASSWORD_ADD,_arr_32);
        EEPROM.put(DV_PASSWORD_ADD,_arr_32);
        _password.toCharArray(_arr_10,_password.length()+1);
        EEPROM.put(WF_KEY_ADD,_arr_10);
        EEPROM.put(SERVER_IP_ADD,_arr_ip);
        EEPROM.put(SERVER_PORT_ADD,_port);
        EEPROM.write(USED_DHT_ADD,1);
        EEPROM.write(DHT_TYPE_ADD,22);
        EEPROM.write(DHT_PIN_ADD,0);
        EEPROM.write(WF_CONNECT_TIMEOUT_ADD,30);
        EEPROM.write(WF_REQUEST_TIMEOUT_ADD,30);
        EEPROM.write(FIRST_SETUP,9);
    }
    EEPROM.end();
    DEBUGln_HBI(F("Close EEPROM"));
};