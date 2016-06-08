#include "WifiBinh.h"
// Process GET request
void ESPHB::_GET(String *_val, String *_request,String _key){
    int g_start,g_compare,g_end,i;
    g_start = _request->indexOf(_key);
    g_compare = _request->indexOf("=",g_start);
    g_end = _request->indexOf("&&",g_compare);
    if(g_end<0){
      g_end = _request->indexOf(" ",g_compare);
    }
    if(g_start<0){
      *_val="";
    }else{
      *_val=_request->substring(g_compare+1, g_end);
    }
}

// Process EEPROM
 Init EEPROM with size (bytes)
void ESPHB::StoreInit(int _size){	
	EEPROM.begin(_size);
}

void ESPHB::Init(void){
	StoreInit(1024);
	EEPROM.get(WifiIPAddr,ip);
	EEPROM.get(WifiInfoAddr,wifi);
	EEPROM.get(DeviceInfoAddr,device);
}

void ESPHB::IPtoString(IPAddress _ip,String * _s){	// chuyển đổi IP dạng struct IPAddress sang String
	*_s=String(_ip[0],DEC)+"."+String(_ip[1],DEC)+"."+String(_ip[2],DEC)+"."+String(_ip[3],DEC); // build String IP from array 4 bytes c_ip[].
}

void ESPHB::StringtoIP(String *_s,IPAddress *_ip){	// chuyển đổi IP dạng struct IPAddress sang String
	int _start=0;	// vị trí xuất phát String IP
    int _dot=*_s.indexOf('.');	// vị trí dấu . đầu tiên
    int _ends=*_s.length();	// tổng chiều dài String IP
    for(char k=0;k<4;k++){
        String ipk=*_s.substring(_start,_dot);	// octet 1: từ vị trí xuất phát đến dấu . đầu tiên
        *_ip[k]=ipk.toInt();	// convert String to Int
        _start=_dot+1;	// dấu . đầu thành điểm xuất phát
        if(k<3){	
			_dot=*_s.indexOf('.',_start);}
            else{	// tại octet 4(octet cuối) điểm kết thúc là chiều dài String IP(ends)
              _dot=_ends;};
    }	
}

void ESPHB::StoreIP(String _ip,String _mask,String _gateway){	// Hàm check và lưu IP vào EEPROM (_ip: ip cần setup, *_return kết quả chuỗi IP trả về sau lưu)
	WifiIP	_ip_tosave
	StringtoIP(&_ip,_ip_tosave->ip);
	StringtoIP(&_mask,_ip_tosave->_mask);
	StringtoIP(&_gateway,_ip_tosave->_gateway);
	EEPROM.put(WifiIPAddr,_ip_tosave);
	EEPROM.commit();
}

void ESPHB::GetIP(String *_s){
	IPtoString(ip.ip,*_s);
}
void ESPHB::GetMask(String *_s){
	IPtoString(ip.mask,*_s);
}
void ESPHB::GetGateway(String *_s){
	IPtoString(ip.gateway,*_s);
}
void ESPHB::GetSsid(String *_s){
	*_s=wifi.ssid
}
void ESPHB::GetPassword(String *_s){
	*_s=wifi.password
}
void ESPHB::GetSerial(String *_s){
	*_s=device.serial
}
void ESPHB::GetKey(String *_s){
	*_s=device.key
}
void ESPHB::GetAdmin(String *_s){
	*_s=device.admin
}

