#include "Bdef.h"

//////////////////////////////////////
///             SYSTEM	           ///
//////////////////////////////////////

ESPHB::ESPHB(unsigned char _ledpin){
	LEDSTATUS=_ledpin;
	pinMode(LEDSTATUS, OUTPUT);
};
// Start up
void ESPHB::Startup(void){
    EEPROMRead(0,&DV_INF);
    EEPROMRead(50,&WF_INF);
    if(DV_INF.DEBUG==1){
        DEBUG = true;
    }else{
        DEBUG = false;
    }
    DEBUG = true;
    if(DV_INF.FIRST_START!=1)  
        Restore();  // Set defaults on first startup
    if(DV_INF.USE_DHT==1){
        dht.init(DV_INF.DHT_PIN, DV_INF.DHT_TYPE, 11,DV_INF.DEBUG);
    }
}
// Save defaults setup to EEPROM (Restore)
void ESPHB::Restore(void){
    EEPROM.begin(MAX_EEPROM_SIZE);
    for(uint16_t i=MAX_EEPROM_SIZE; i>0; i--){
        EEPROM.write(i-1,0);
    }
    EEPROM.end();
    DV_INF.DEBUG=1;
    DV_INF.FIRST_START=1;    // 1
    StringToArray(&DEFAULT_KEY, DV_INF.DV_ADMIN, MAX_ADMIN_LEN);
    WF_INF.IS_STATICIP = 0;
    WF_INF.WF_CONN_TIMEOUT = 60;
    WF_INF.WF_STATICIP = 0;
    WF_INF.MAX_REQUEST_TIMEOUT = 6;
    WF_INF.MASTER_SERVER_PORT = 80;
    
    StringToArray(&null, WF_INF.WF_SSID, MAX_SSID_LEN);
    StringToArray(&null, WF_INF.WF_PASSWORD, MAX_PASSWORD_LEN);
    StringToArray(&DEFAULT_KEY, WF_INF.WF_KEY, MAX_KEY_LEN);
    StringToArray(&DEFAULT_APPASSWORD, WF_INF.AP_PASSWORD, MAX_PASSWORD_LEN);
    StringToArray(&null, WF_INF.MASTER_SERVER, MAX_SERVER_LEN);
    EEPROMSave(0,&DV_INF);
    EEPROMSave(50,&WF_INF);
    ESP.restart();
}

//////////////////////////////////////////
///     Group function for Serial	   ///
//////////////////////////////////////////
void ESPHB::SerialEvent(void) {
	while (Serial.available()){
		char inChar = (char)Serial.read();
		SERIAL_RECEIVER += inChar;
		if (inChar == '\n') {SERIAL_COMPLETE = true;}   
		if(SERIAL_COMPLETE){
			String command=SERIAL_RECEIVER.substring(0,SERIAL_RECEIVER.indexOf("=="));
			String value=SERIAL_RECEIVER.substring(SERIAL_RECEIVER.indexOf("==")+2,SERIAL_RECEIVER.length()-1);
			if((command=="login")&&(!LOGINED)){
				if(value!=String(DV_INF.DV_ADMIN)){
					Serial.println(FPSTR(lb_LOGIN_FAILED));
				}else{
					Serial.println(FPSTR(lb_LOGIN_SUCCESS));
					LOGINED=true;					
				}
			}	
			else if((command=="status")&&LOGINED){
                Serial.print(FPSTR(lb_KEY));    Serial.println(WF_INF.WF_KEY);
				Serial.print(FPSTR(lb_SERIAL)); Serial.println(DV_INF.DV_SERIAL);
				Serial.print(FPSTR(lb_SSID));   Serial.println(WF_INF.WF_SSID);
                Serial.print(FPSTR(lb_PASSWORD));   Serial.println(WF_INF.WF_PASSWORD);
                Serial.print(FPSTR(lb_SERVER));   Serial.println(WF_INF.MASTER_SERVER);
                Serial.print(FPSTR(lb_SERVER));   Serial.println(WF_INF.MASTER_SERVER_PORT);
                Serial.print(FPSTR(lb_CONNECT_STATUS)); if(CONNECTED)   Serial.println(FPSTR(lb_CONNECTED)); else   Serial.println(FPSTR(lb_FAILED_CONNECT));
                Serial.print(FPSTR(lb_STA));    Serial.print(FPSTR(lb_MAC_ADDRESS));    Serial.println(WiFi.macAddress());
                Serial.print(FPSTR(lb_STA));    Serial.print(FPSTR(lb_IP));    Serial.println(WiFi.localIP());	
			}
			else if((command=="debug")&&LOGINED){
                DV_INF.DEBUG=true;
				EEPROMSave(0,&DV_INF);
                Serial.println(FPSTR(lb_DEBUG_ENABLE));
				ESP.restart();
			}
			else if((command=="normal")&&LOGINED){
                DV_INF.DEBUG=false;
				EEPROMSave(0,&DV_INF);
                Serial.println(FPSTR(lb_DEBUG_ENABLE));
				ESP.restart();
			}
			else if((command=="reboot")&&LOGINED){
				Serial.println(FPSTR(lb_REBOOT));
				ESP.restart();
			}
			else if((command=="restore")&&LOGINED){
				Serial.println(FPSTR(lb_RESTORE));
				Restore();
				ESP.restart();
			}
			else if((command=="serial")&&LOGINED){
				if(StringToArray(&value, DV_INF.DV_SERIAL, MAX_SERIAL_LEN)){
                    EEPROMSave(0,&DV_INF);
					Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                    Serial.print(FPSTR(lb_SERIAL));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);	
				}else{
                    Serial.print(FPSTR(lb_CHANGE_FAILED));
                    Serial.print(FPSTR(lb_SERIAL));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);
                };
			}
			else if((command=="ssid")&&LOGINED){
				if(StringToArray(&value, WF_INF.WF_SSID, MAX_SSID_LEN)){
                    EEPROMSave(50,&WF_INF);
					Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                    Serial.print(FPSTR(lb_SSID));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);	
				}else{
                    Serial.print(FPSTR(lb_CHANGE_FAILED));
                    Serial.print(FPSTR(lb_SSID));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);
                };
			}
			else if((command=="password")&&LOGINED){
				if(StringToArray(&value, WF_INF.WF_PASSWORD, MAX_PASSWORD_LEN)){
                    EEPROMSave(50,&WF_INF);
					Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                    Serial.print(FPSTR(lb_PASSWORD));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);	
				}else{
                    Serial.print(FPSTR(lb_CHANGE_FAILED));
                    Serial.print(FPSTR(lb_PASSWORD));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);
                };
			}
			else if((command=="key")&&LOGINED){
				if(StringToArray(&value, WF_INF.WF_KEY, MAX_KEY_LEN)){
                    EEPROMSave(50,&WF_INF);
					Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                    Serial.print(FPSTR(lb_KEY));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);	
				}else{
                    Serial.print(FPSTR(lb_CHANGE_FAILED));
                    Serial.print(FPSTR(lb_KEY));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);
                };
			}
			else if((command=="admin")&&LOGINED){
				if(StringToArray(&value, DV_INF.DV_ADMIN, MAX_ADMIN_LEN)){
                    EEPROMSave(0,&DV_INF);
					Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                    Serial.print(FPSTR(lb_ADMIN_PASSWORD));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);	
				}else{
                    Serial.print(FPSTR(lb_CHANGE_FAILED));
                    Serial.print(FPSTR(lb_ADMIN_PASSWORD));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);
                };
			}
			else if((command=="server")&&LOGINED){
				if(StringToArray(&value, WF_INF.MASTER_SERVER, MAX_SERVER_LEN)){
                    EEPROMSave(50,&WF_INF);
					Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                    Serial.print(FPSTR(lb_SERVER));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);	
				}else{
                    Serial.print(FPSTR(lb_CHANGE_FAILED));
                    Serial.print(FPSTR(lb_SERVER));
                    Serial.print(FPSTR(lb_TO));
					Serial.println(value);
                };
			}
			else if((command=="port")&&LOGINED){
                WF_INF.MASTER_SERVER_PORT=value.toInt();
                EEPROMSave(50,&WF_INF);
				Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                Serial.print(FPSTR(lb_SERVER_PORT));
                Serial.print(FPSTR(lb_TO));
			    Serial.println(value);
			}else if((command=="setDHT")&&LOGINED){
                // String setup: setDHT==ON/OFF-type-pin
                // ex: setDHT==ON-22-14
                uint8_t _dhttype,_dhtpin,_sepa,_nextsepa;
                _sepa = value.indexOf("-");
                _nextsepa = value.indexOf("-",_sepa);
                String _dhtset=value.substring(0,sepa);
                _dhttype = (value.substring( _sepa + 1, _nextsepa)).toInt();
                _dhtpin = (value.substring( _nextsepa + 1, value.length())).toInt();
                if(_dhtset=="ON"){
                    DV_INF.USE_DHT = 1;
                    DV_INF.DHT_TYPE = _dhttype;
                    DV_INF.DHT_PIN = _dhtpin;
                }else{
                    DV_INF.USE_DHT = 0;
                }
                
                
                WF_INF.MASTER_SERVER_PORT=value.toInt();
                EEPROMSave(50,&DV_INF);
				Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                Serial.print(FPSTR(lb_SERVER_PORT));
                Serial.print(FPSTR(lb_TO));
			    Serial.println(value);
			}
			else if((command=="ip")&&LOGINED){
				WF_INF.WF_STATICIP = StringToIPAdress(value);
                EEPROMSave(50,&WF_INF);
				Serial.print(FPSTR(lb_CHANGE_SUCCESS));
                Serial.print(FPSTR(lb_STATIC));
                Serial.print(FPSTR(lb_IP));
                Serial.print(FPSTR(lb_TO));
				Serial.println(value);	
			}
			else if((command=="DHCP")&&LOGINED){
                if(WF_INF.IS_STATICIP!=1){
                    WF_INF.IS_STATICIP = 1;
                    Serial.println(FPSTR(lb_DHCP_ENABLE));
                }else{
                    WF_INF.IS_STATICIP = 0;
                    Serial.println(FPSTR(lb_DHCP_DISABLE));	
                }
                EEPROMSave(50,&WF_INF);
			}else{
				Serial.println(FPSTR(lb_COMMAND_ERROR));
			}
			SERIAL_RECEIVER="";
			SERIAL_COMPLETE = false;
		}
	}
}





//////////////////////////////////////
///     Group function for HTML	   ///
//////////////////////////////////////
// Event when ESP8266 server receive request 
boolean ESPHB::CheckArlert(String *request){
    if((request->indexOf(">ON"))>0){
        if(DEBUG)   Serial.print(F("alert"));
        return true;       
    }else{
        return false;
    }
}

// Handler Event when ESP8266 server receive request
String ESPHB::httpHandlerEvent(String *request){
	String cssid,cpassword,ckey,cserver,respone="";
	GETValue(request,"key",&ckey);
	GETValue(request,"ssid",&cssid);
	GETValue(request,"password",&cpassword);
	GETValue(request,"server",&cserver);
	if((ckey==String(WF_INF.WF_KEY))&&(cssid.length()<=MAX_SSID_LEN)&&(cpassword.length()<=MAX_PASSWORD_LEN)&&(cserver.length()<=MAX_SERVER_LEN)){
		StringToArray(&cssid, WF_INF.WF_SSID, MAX_SSID_LEN);
        StringToArray(&cpassword, WF_INF.WF_PASSWORD, MAX_PASSWORD_LEN);
        StringToArray(&cserver, WF_INF.MASTER_SERVER, MAX_SERVER_LEN);
        EEPROMSave(50,&WF_INF);        
		jsonEncode(ONEJSON,&respone,"result",F("Success"));
		if(DEBUG){
            Serial.println(FPSTR(lb_CHANGE_SUCCESS));
            Serial.print(FPSTR(lb_SSID));
            Serial.print(FPSTR(lb_TO));
            Serial.println(cssid);
            Serial.print(FPSTR(lb_PASSWORD));
            Serial.print(FPSTR(lb_TO));
            Serial.println(cpassword);
            Serial.print(FPSTR(lb_SERVER));
            Serial.print(FPSTR(lb_TO));
            Serial.println(cserver);            
        };
	}else{
        jsonEncode(ONEJSON,&respone,"result",F("Error"));
    }
    return respone;
}
