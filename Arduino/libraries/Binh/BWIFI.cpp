#include "BWIFI.h"
//////////////////////////////////////
///     Group function for WiFi	   ///
//////////////////////////////////////
void BWIFI::init(){
    readData();
}
uint32_t BWIFI::getServer(void){
    return server_ip;
};
uint16_t BWIFI::getServerPort(void){
    return server_port;
};
uint8_t BWIFI::getRequestTimeout(void){
    return wf_request_timeout;
}
// Connect to wifi
void BWIFI::connect(void){
	CONNECTED=false;
	lastConnect=millis();	// khởi tạo biến timeout
    DEBUGln_HBI(FPSTR(lb_CONNECTING));
    DEBUG_HBI(FPSTR(lb_SSID));    DEBUGln_HBI(wf_ssid);
    DEBUG_HBI(FPSTR(lb_PASSWORD));    DEBUGln_HBI(wf_password);
	WiFi.begin(wf_ssid, wf_password);	// kết nối tới mạng wifi
};
boolean BWIFI::checkConnected(void){
	if(WiFi.status() != WL_CONNECTED){
        DEBUG_HBI(FPSTR(lb_DOT));
        if(getConnectTimeOut()>wf_connect_timeout){
            DEBUGln_HBI(F("-"));
        }
		CONNECTED=false;
	}else{
        CONNECTED=true;
        DEBUG_HBI(F("+"));
    }
    return CONNECTED;
}
void BWIFI::reConnect(void){
	if(!CONNECTED&&(getConnectTimeOut()>wf_connect_timeout)){
		connect();
	}
}
uint8_t BWIFI::getConnectTimeOut(void){
    return (uint8_t)1000*(millis()-lastConnect);
}
void BWIFI::setSTAMode(void){
    APMODE = false;
	WiFi.mode(WIFI_STA);
    DEBUG_HBI(FPSTR(lb_STAMODE));
}
void BWIFI::setAPMode(void){
    APMODE = true;
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(ap_ssid, ap_password);
    DEBUG_HBI(FPSTR(lb_AP));
    DEBUG_HBI(FPSTR(lb_MAC_ADDRESS));
    DEBUGln_HBI(WiFi.softAPmacAddress());
    DEBUG_HBI(FPSTR(lb_AP));
    DEBUG_HBI(FPSTR(lb_IP));
    DEBUGln_HBI(WiFi.softAPIP()); 
    DEBUG_HBI(ap_ssid);  
    DEBUG_HBI(ap_password); 
}


