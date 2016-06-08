#include "Binh.h"
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
// Init EEPROM with size (bytes)
void ESPHB::StoreInit(int _size){	
	EEPROM.begin(_size);
}
void ESPHB::Init{
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
}

unsigned char ESPHB::read1byte_EEPROM(unsigned char _address) {	
  return EEPROM.read(_address);  // đọc 1 byte từ EEPROM có địa chỉ _address
}
// Read max length of data
int ESPHB::MAX_LEN_EEPROM(unsigned char _content) {
	return EEPROM.read(_content+1);
}
// Read string from EEPROM
void ESPHB::read_EEPROM(unsigned char _content, String *_value) {
  unsigned char len = EEPROM.read(_content);  // Read leghth of array in fisrt byte of block content in EEPROM
  *_value="";
  for (unsigned char i = 0; i < len; i++) {
	*_value+=EEPROM.read(i + _content + 2);
  }
}
// Read array char from EEPROM
void ESPHB::read_EEPROM(unsigned char _content, char *_value) {
  unsigned char len = EEPROM.read(_content);  // Read leghth of array in fisrt byte of block content in EEPROM
  for (unsigned char i = 0; i < len; i++) {
	_value[i]=EEPROM.read(i + _content + 2);
  }
}
// Save to EEPROM
void ESPHB::update1byte_EEPROM(unsigned char _address, unsigned char _value){
    EEPROM.write(_address, _value);  // Write 1 byte to EEPROM at _address
    EEPROM.commit();// Save from RAM to EEPROM
}
boolean ESPHB::update_EEPROM(unsigned char _address, unsigned char _maxlen, String _text){
  char len = _text.length() + 1;  // leghth of array (=leghth of string +1) will be write to EEPROM; +1 because: end of array is "\0"
  char arr[len];  // Init an array
  _text.toCharArray(arr, len);  // Convert string to array
  if (len <= _maxlen) {  // Write array to EEPROM when leghth of array <= max leghth in EEPROM
    EEPROM.write(_address, len);  // Write leghth of array in fisrt byte of block content in EEPROM
    for (unsigned char i = 0; i < len; i++) {
      EEPROM.write(i + _address + 1, arr[i]);  // Write byte by byte, start from second byte of block content in EEPROM
    }
    EEPROM.commit();// Save from RAM to EEPROM
    return true;  // Return true when leghth of array <= max leghth in EEPROM
  } else {
    return false;  // Return false when leghth of array > max leghth in EEPROM and not save to EEPROM
  }
}
boolean ESPHB::save_EEPROM(unsigned char _content,String _value){
	char len = _value.length() + 1;  // leghth of array (=leghth of string +1) will be write to EEPROM; +1 because: end of array is "\0"
	char arr[len];  // Init an array
	_value.toCharArray(arr, len);  // Convert string to array
	if(MAX_LEN_EEPROM(_content)>=len)	{	// Write array to EEPROM when leghth of array <= max leghth in EEPROM
		EEPROM.write(_content, len);  // Write leghth of array in fisrt byte of block content in EEPROM
		for (unsigned char i = 0; i < len; i++) {
			EEPROM.write(i + _content + 2, arr[i]);  // Write byte by byte, start from second byte of block content in EEPROM
		}
		update_EEPROM(_address, _maxlen, _value);
		char *_a = read_EEPROM(__serial_adrr);  *_return = _a;  free(_a);  // Read serial of device
		return true;
	}
	else return false;
};
boolean ESPHB::save_verify(unsigned char _addr,unsigned char _value){
	return (EEPROM.read(_addr)==_value)?true:false;
}



void ESPHB::requestEncode(String * _s,String _server,String _link,int _position,  String _key, String _val){	// header http + content
	switch (_position) {
        case ONEGET:
        case FIRSTGET:
			*_s =*_s+"GET "+_link;
			if((_position==ONEGET)||(_position==FIRSTGET)){*_s=*_s+"?"+_key+"="+_val;}
			if(_position==ONEGET){*_s =*_s+http_header_host+_server+http_header_info;}
            break;
        case NEXTGET:
			requestAdd(_s,_key,_val);
            break;
        case LASTGET:
			requestAdd(_s,_key,_val);
            *_s =*_s+http_header_host+_server+http_header_info;
            break;
    };
}

void ESPHB::DynDNS(String _server,String _link,unsigned long _period_ddns){
	char _len = _server.length() + 1;
	char _ddns_host[_len];
	_server.toCharArray(_ddns_host, _len);
	unsigned long _current_time=millis();
	unsigned char _time_out=0;
	String ddns_req="";
	if((_current_time - dyndns_time)>=_period_ddns){
		WiFiClient client3;
		requestEncode(&ddns_req,_server,_link,ONEGET,"serial",serial);
		if (!client3.connect(_ddns_host, 80)) {
		debug(_label_conn_fail);
		delay(500);
		_time_out++;
		if(_time_out>times_timeout_dyndns){dyndns_time=_current_time;}
		return;
    };
	debug(&ddns_req);
     client3.print(ddns_req);
      _ddns_receive="";
   while(client3.available()){_ddns_receive = client3.readString();}
   debug(&_ddns_receive);
   client3.stop();
   dyndns_time=_current_time;
   };	
}


// Complete public
void ESPHB::debug(String * _s){
	if(isdebug){Serial.println(*_s);};
}
void ESPHB::debug(String _s){
	if(isdebug){Serial.println(_s);};
}
void ESPHB::debug(uint32_t _s){
	if(isdebug){Serial.println(_s);};
}
void ESPHB::set_debug(boolean _debug){
	if(_debug){update1byte_EEPROM(__debug, 1);}
	else{update1byte_EEPROM(__debug, 0);};
}
void ESPHB::restore(void){
	String _null="";
	IPAddress _nil=(0,0,0,0);
	save_ip(_nil,__IP_adrr);
	save_ip(_nil,__gateway_adrr);
	save_ip(_nil,__mask_adrr);
	update1byte_EEPROM(__first_start, 0);
	update1byte_EEPROM(__ip_static, 0);
	save(__ssid,_null);
	save(__password,_null);
}


void ESPHB::read_configs(configs *_cf,defaults *_df){
	firststart = read1byte_EEPROM(__first_start);
	isdebug = read1byte_EEPROM(__debug);
	ipstatic=read1byte_EEPROM(__ip_static);
	char *_a = read_EEPROM(__serial_adrr); serial=_a;  free(_a);  // Read serial of device
	char *_c = read_EEPROM(__ssid_adrr); ssid = _c;  free(_c);  // Read ssid of wifi
	char *_d = read_EEPROM(__password_adrr); password = _d;  free(_d);  // Read password of wifi
	if (!firststart) { // write serial
		update_EEPROM(__serial_adrr, __serial_max, _df->serial);
		update_EEPROM(__key_adrr, __key_max, _df->key);
		update_EEPROM(__admin_adrr, __admin_max, _df->admin);
		debug("\nUpdated: serial - " + _df->serial + "key: " + _df->key +"Admin key: "+_df->admin);
		update1byte_EEPROM(__first_start, 1);
	}
	char *_b = read_EEPROM(__key_adrr);  _cf->key = _b;  free(_b);  // Read key to connect to device
	char *_e = read_EEPROM(__admin_adrr);  _cf->admin = _e;  free(_e);  // Read admin key of device
	_cf->serial = serial;
	_cf->ssid = ssid;
	_cf->password = password;
	for (unsigned char i = 0; i < 4; i++) {
	ip[i] = read1byte_EEPROM(i + __IP_adrr + 1);
    _cf->ip[i] = ip[i];  // Read byte to byte, start from second byte of block content in EEPROM
	}
	for (unsigned char i = 0; i < 4; i++) {
	gateway[i] = read1byte_EEPROM(i + __gateway_adrr + 1); 
    _cf->gateway[i] = gateway[i];  // Read byte to byte, start from second byte of block content in EEPROM
	}
	for (unsigned char i = 0; i < 4; i++) {
	mask[i] = read1byte_EEPROM(i + __mask_adrr + 1);
    _cf->mask[i] = mask[i];  // Read byte to byte, start from second byte of block content in EEPROM
	}
};
boolean ESPHB::wifi_connect(){
if(ssid!=""){
	unsigned char _timeout=0;	// khởi tạo biến timeout
	debug("Connecting to ");
	char _sta_ssid[(ssid).length()+1];	// chuyển đổi String ssid, password sang char array
	char _sta_password[(password).length()+1];
	ssid.toCharArray(_sta_ssid, ssid.length()+1);
	password.toCharArray(_sta_password, password.length()+1);
	debug(_sta_ssid);
	debug(_sta_password);
	WiFi.begin(_sta_ssid, _sta_password);	// kết nối tới mạng wifi
	while (WiFi.status() != WL_CONNECTED) {	// nếu chưa kết nối được
		delay(500); if(isdebug){Serial.print(".");}	// xuất ký tự . mỗi 0.5s
		_timeout++;	// tăng timeout
		apmode=false;
		if(_timeout>__timeout_STA){	// nếu timeout > thời gian cho phép timeout_STA
			apmode=true;	// bật cờ apmode
			break;
		}
	}
	if((!ipstatic)&&(!apmode)){
		String _tmp="";
		save_ip(WiFi.localIP(),__IP_adrr);
		save_ip(WiFi.gatewayIP(),__gateway_adrr);
		save_ip(WiFi.subnetMask(),__mask_adrr);
		IPtoString(WiFi.localIP(),&_tmp);debug("New IP: "+_tmp);
		IPtoString(WiFi.gatewayIP(),&_tmp);debug("New gateway: "+_tmp);
		IPtoString(WiFi.subnetMask(),&_tmp);debug("New submask: "+_tmp);
		update1byte_EEPROM(__ip_static,1);
		ipstatic=true;
    }
    else if(ipstatic&&(!apmode)){
      WiFi.config(ip,gateway,mask);
    };	
	if(!apmode){	// nếu cờ apmode chưa bật( đã kết nối tới wifi)
		debug("");
		debug("WiFi connected to:");
		debug(_sta_ssid);
    }
}
	return apmode;	// trả về true nếu kết nối và false chưa kết nối
};
boolean ESPHB::wifi_connect(String _ssid, String _password, String *_ip,String * _result){
	unsigned char _timeout=0;
	String _tmp="";
	boolean _receiveip=true;
	debug("Connecting to ");
	// convert String of ssid, password to array (as update_EEPROM)
	char _sta_ssid[(_ssid).length()+1];
	char _sta_password[(_password).length()+1];
	_ssid.toCharArray(_sta_ssid, _ssid.length()+1);
	_password.toCharArray(_sta_password, _password.length()+1);
	debug(_sta_ssid);
	debug(_sta_password);
	WiFi.begin(_sta_ssid, _sta_password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500); if(isdebug){Serial.print(".");}
		_timeout++;
		if(_timeout>__timeout_STA){
			_receiveip=false;
			*_result="failed";
			*_ip="";
			break;
		}
	}
	if(_receiveip){
		debug("");
		debug("WiFi connected to:");
		debug(_sta_ssid);
		IPtoString(WiFi.localIP(),&_tmp);
		save_ip(WiFi.localIP(),__IP_adrr);
		save_ip(WiFi.gatewayIP(),__gateway_adrr);
		save_ip(WiFi.subnetMask(),__mask_adrr);
		update1byte_EEPROM(__ip_static,1);
		debug("IP: "+_tmp);
		*_ip=_tmp;
		*_result="connected";
		save(__ssid,_ssid);
		save(__password,_password);
    }
	return !_receiveip;
}
void ESPHB::wifi_APconnect(defaults *_df){
		String _tmp;
		char _ap_ssid[(_df->apssid).length()+1];
		char _ap_password[(_df->appassword).length()+1];	
		(_df->apssid).toCharArray(_ap_ssid, (_df->apssid).length()+1);
		(_df->appassword).toCharArray(_ap_password, (_df->appassword).length()+1);	
		apmode=true;
		WiFi.mode(WIFI_AP_STA);
		WiFi.softAP(_ap_ssid, _ap_password);
		debug("Cannot connected, you can connect to:");
		IPtoString(WiFi.softAPIP(),&_tmp);
		debug(_tmp);
}

boolean ESPHB::save_ip(String _ip,String *_return){	// Hàm check và lưu IP vào EEPROM (_ip: ip cần setup, *_return kết quả chuỗi IP trả về sau lưu)
	String _curent_ip="";	// khởi tạo ip tạm
	*_return="";	// trả về = ""
	IPAddress _ip_tosave;	// khởi tạo struct IPAddress để lưu
	int _start=0;	// vị trí xuất phát String IP
    int _dot=_ip.indexOf('.');	// vị trí dấu . đầu tiên
    int _ends=_ip.length();	// tổng chiều dài String IP
    for(char k=0;k<4;k++){
        String ipk=_ip.substring(_start,_dot);	// octet 1: từ vị trí xuất phát đến dấu . đầu tiên
        _ip_tosave[k]=ipk.toInt();	// convert String to Int
        _start=_dot+1;	// dấu . đầu thành điểm xuất phát
        if(k<3){	
			_dot=_ip.indexOf('.',_start);}
            else{	// tại octet 4(octet cuối) điểm kết thúc là chiều dài String IP(ends)
              _dot=_ends;};
    }
	WiFi.config(_ip_tosave,gateway,mask);	// thiết lập lại địa chỉ IP
	IPtoString(WiFi.localIP(),&_curent_ip);	// lấy địa chỉ IP sau khi thiết lập
	if(_curent_ip==_ip){	// nếu địa chỉ IP nhận được == địa chỉ IP setup
		save_ip(_ip_tosave,__IP_adrr);	// lưu ip mới vào EEPROM
		for (unsigned char i = 0; i < 4; i++) {	// Đọc IP vừa lưu từ EEPROM
			_ip_tosave[i] = read1byte_EEPROM(i + __IP_adrr + 1);
		}
		IPtoString(_ip_tosave,_return);	// trả về String ip sau khi được lưu
		return true;	// return true hàm
	}else{
		*_return=_ip;	// trả về String ip ban đầu đem setup
		return false;
	}
}
void ESPHB::save_ip(IPAddress _ip,unsigned char _address){	// Lưu IP dạng truyển vào là struct IPAddress
	update1byte_EEPROM(_address, 4);	// lưu độ dài vào EEPROM, 4 octet
    for(char k=0;k<4;k++){
        update1byte_EEPROM(_address+1+k,_ip[k]);	// lưu từng octet vào EEPROM
    };
};
void ESPHB::toggle_pin(String _pinname, String _value,String *_return_key,String *_return_value){	// toggle pinout
  digitalWrite(get_pin(_pinname), pin_value(_value));
  *_return_key=_pinname;
  if(digitalRead(get_pin(_pinname))){*_return_value="ON";}else{*_return_value="OFF";}
}
void ESPHB::jsonEncode(int _position, String * _s, String _key, String _val){	// header http + content
	switch (_position) {
        case ONEJSON:
        case FIRSTJSON:
			*_s += http_header_respone;
            *_s += "{\r\n";
            jsonAdd(_s,_key,_val);
            *_s+= (_position==ONEJSON) ? "\r\n}" : ",\r\n";
            break;
        case NEXTJSON:
            jsonAdd(_s,_key,_val);
            *_s+= ",\r\n";
            break;
        case LASTJSON:
            jsonAdd(_s,_key,_val);
            *_s+= "\r\n}";
            break;
    };
}

// complete private


unsigned char ESPHB::get_pin(String _pin){	// Get pin name from string to int
	return _pin.substring(3,_pin.length()).toInt();
}
boolean ESPHB::pin_value(String _value){	// Get pin value from string to boolean
	if(_value=="ON"){return HIGH;}else if(_value=="OFF"){return LOW;};
}
void ESPHB::jsonAdd(String *_s, String _key,String _val) {
    *_s += '"' + _key + '"' + ":" + '"' + _val + '"';
}
void ESPHB::requestAdd(String *_s, String _key,String _val) {
    *_s += "&&" + _key + "=" + _val;
}
