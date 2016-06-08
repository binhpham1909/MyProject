#include "BClient.h"

// Send GET Request and receive respone from server
// Ex html GET: "GET /hostlink/local/file.php?key1=value1&&key2=value2 HTTP/1.1\r\nHost: 192.168.0.3\r\nConnection: close\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUser-Agent: wifi-switch\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: vi-VN,vi;q=0.8,fr-FR;q=0.6,fr;q=0.4,en-US;q=0.2,en;q=0.2\r\n\r\n"
boolean BClient::sendRequest(uint32_t _server,uint16_t port, String *_tosend, uint8_t _request_timeout){
    if(!sendSuccess)    return false;
    respone="";
    requestTimeOut = _request_timeout*1000;
    sendSuccess = false;
    if (!client.connect(_server, port)) {
        DEBUGln_HBI(FPSTR(lb_FAILED_CONNECT));
        return false;
    };
    client.print(*_tosend);
    lastCheckTime = millis();
    DEBUG_HBI(F("\n"));
    DEBUG_HBI(*_tosend);                                                                                                               
	return true;
}
boolean BClient::checkRespone(void){
    if((millis()-lastCheckTime)>requestTimeOut){
       client.stop();
       sendSuccess = true;
       return true; 
    }   
    if(!client.available())
        return false;
    else
        return true;
}

void BClient::getRespone(String &_respone){
    if(!sendSuccess){
        _respone = "";
        while (client.available()) { 
            _respone = client.readString();
        }
        client.stop();
        DEBUG_HBI(_respone);
        sendSuccess = true;          
    }
}