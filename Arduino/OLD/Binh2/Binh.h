/*
Libraries of esp8266 connect to wifi, IOT of Pham Huuu Binh
functions include:
public:
boolean wifi_Init(char * STAssid,char * STApassword,char * APssid,char * APpassword);
	- Connect to wifi network with STAssid, STApassword. If time for connect > _timeout_STA then auto switch to 
	AP mode. In AP mode generate wifi network with APssid, APpassword and IP is 192.168.4.1. Return true if in 
	STA mode and false in AP mode.
	
void start_EEPROM(int size);
	- Init EEPROM with size in byte
	
void read_configs(configs *cf,String stSerial,String stKey);
	- Read configs from EEPROM and return in pointer "cf" with: serial, key, ssid, password, ip. "stSerial" is 
	factory serial of device you want set, "stKey" is default factory key connect of device.
	
boolean save_configs(unsigned char content,String value);
	- Save configs to EEPROM

void save_ipstr(String _ip);
	- save IP format String _ip (eg: 192.168.1.8) to EEPROM
	
void save_ipaddr(IPAddress _ip);
	- save IP format IPAddress _ip (eg: _ip(192,168,1,8) to EEPROM
	
void toggle_pin(String pinname, String value);
	- Turn ON, OFF GPIO pin with format: toggle_pin(pinxx,"ON"/"OFF");

void getDecode(request *s, String http_rq);
		void jsonEncode(int pos, String * s, String key, String val);
*/

#ifndef Binh_h
#define Binh_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// EEPROM byte:
// 0: bien khoi dong - 0: khoi dong lan dau; 1: khoi dong lan sau
// x0: so ky tu trong chuoi
// 10-19: num byte of SERIAL  (max 9)
// 20-29: num byte of key  (max 9)
// 30-69: num byte of ssid  (max 39)
// 70-109: num byte of password (max 39)
// 110-119: num byte of static ip (max 39)
// 120-129: num byte of masterkey  (max 9)
// arrtostr: String var; ga'n: var=arr

#define __first_start 0		// Lan khoi dong dau tien
#define __ip_static 1		// Chon setup static IP cho thiet bi
#define __debug 2		// Chon setup static IP cho thiet bi
#define __serial_adrr 10	// Block dia chi Serial thiet bi 10byte, 1 byte chieu dai, 8 byte noi dung, 1 byte \0
#define __serial_max 9	// max 9 = 8 byte noi dung + 1 byte \0
#define __key	1
#define __key_adrr 20	// Block dia chi key thiet bi 10byte, 1 byte chieu dai, 8 byte noi dung, 1 byte \0
#define __key_max 9		// max 9 = 8 byte noi dung + 1 byte \0
#define __ssid	2
#define __ssid_adrr 30	// Block dia chi luu ssid 40byte, 1 byte chieu dai, 38 byte noi dung, 1 byte \0
#define __ssid_max 39	// max 39 = 38 byte noi dung + 1 byte \0
#define __password	3
#define __password_adrr 70	// Block dia chi luu password wifi 40byte, 1 byte chieu dai, 38 byte noi dung, 1 byte \0
#define __password_max 39	// max 39 = 38 byte noi dung + 1 byte \0
#define __IP	4
#define __IP_adrr 110	// Block dia chi luu static IP 5 byte, 1 byte chieu dai, 4 byte noi dung
#define __IP_max 5		// max 4 = 4 byte noi dung
#define __gateway	5
#define __gateway_adrr 115	// Block dia chi luu static gateway 5 byte, 1 byte chieu dai, 4 byte noi dung
#define __gateway_max 5		// max 4 = 4 byte noi dung
#define __mask	6
#define __mask_adrr 120	// Block dia chi luu static gateway 5 byte, 1 byte chieu dai, 4 byte noi dung
#define __mask_max 5		// max 4 = 4 byte noi dung
#define __admin	7
#define __admin_adrr 130	// Block dia chi luu password admin service 10byte, 1 byte chieu dai, 8 byte noi dung, 1 byte \0
#define __admin_max 9		// max 9 = 8 byte noi dung + 1 byte \0

#define __timeout_STA	60	// x*0.5s
// Json encode
#define	ONEJSON	1
#define	FIRSTJSON	2
#define	NEXTJSON	3
#define	LASTJSON	4
// Request encode
#define	ONEGET	1
#define	FIRSTGET	2
#define	NEXTGET	3
#define	LASTGET	4
// Label
const int httpPort = 80;
const String _label_ssid = "ssid";
const String _label_password = "password";
const String _label_key = "key";
const String _label_serial = "serial";
const String _label_ip = "ip";
const String _label_reboot = "Rebooting";
const String _label_result = "result";
const String _label_status = "status";
const String _label_online = "online";
const String _label_complete = "complete";
const String _label_error = "error";
const String _label_conn_ok = "Connected";
const String _label_conn_fail = "connection failed";
struct defaults{
	String serial;	// Serial mac dinh thiet bi
	String apssid;	// SSID ap mode mac dinh thiet bi
	String appassword;	// PASSWORD ap mode mac dinh thiet bi
	String key;	// key dieu khien mac dinh thiet bi
	String admin;	// key Admin mac dinh thiet bi
};
struct configs {	// struct chua cac thong so setup thiet bi
  String serial;	// chuoi serial cua thiet bi
  String key;		// chuoi key ket noi dieu khien thiet bi
  String ssid;		// chuoi ssid de ket noi wifi
  String password;	// chuoi mat khau de ket noi wifi
  IPAddress ip;	// static IP cua thiet bi
  IPAddress gateway;	// static IP cua thiet bi
  IPAddress mask;	// static IP cua thiet bi
  String admin;	// mat khau admin service
};
struct http_read {	// struct chua chuoi json sau khi dc tach
  String names;	// key
  String value;	// gia tri
};
struct request{	// struct chua cac thong tin sau khi xu ly GET request
  String command;
  String key;
  String newkey;
  String ssid;
  String password;
  String ip;
  String newip;
  String pin;
  String value;
  String admin;
  String link;
};
class ESPHB	// class chua cac ham xu ly cua thu vien
{
	public:
		void debug(String * _s);	// Print String if debug mode on
		void debug(String _s);	// Print String if debug mode on
		void debug(uint32_t _s);	// Print String if debug mode on
		void set_debug(boolean _debug);	// Set debug mode on or off
		void restore(void);
		void init_store(int _size);	// Start EEPROM for store configs
		void IPtoString(IPAddress _ip,String * _s);
		void read_configs(configs *_cf,defaults *_df);		
		boolean wifi_connect();		
		boolean wifi_connect(String _ssid, String _password, String *_ip,String *_result);
		void wifi_APconnect(defaults *_df);
		boolean save(unsigned char _content,String _value,String *_return);
		boolean save(unsigned char _content,String _value);
		boolean save_ip(String _ip,String *_return);
		void save_ip(IPAddress _ip,unsigned char _address);
		void toggle_pin(String _pinname, String _value,String *_return_key,String *_return_value);
		void jsonEncode(int _position, String * _s, String _key, String _val);
		void requestEncode(String * _s,String _server,String _link,int _position,  String _key, String _val);		
		void DynDNS(String _server,String _link,unsigned long _period_ddns);
		
		void getDecode(request *_s, String _request);
	private:
		boolean isdebug;
		boolean firststart;
		boolean ipstatic;
		boolean apmode=true;
		String serial;
		String ssid;
		String password;
		IPAddress ip;	// static IP cua thiet bi
		IPAddress gateway;	// static IP cua thiet bi
		IPAddress mask;	// static IP cua thiet bi
		unsigned long dyndns_time=0;
		unsigned char times_timeout_dyndns=3;
		String _ddns_receive="";
		String http_header_info="\r\nConnection: close\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUser-Agent: wifi-switch\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: vi-VN,vi;q=0.8,fr-FR;q=0.6,fr;q=0.4,en-US;q=0.2,en;q=0.2\r\n\r\n";
		String http_header_host=" HTTP/1.1\r\nHost: ";
		String http_header_respone="HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: application/json\r\nUser-Agent: Wifi-switch\r\nConnection: close\r\n\r\n";
		void update1byte_EEPROM(unsigned char _address, unsigned char _value);
		boolean update_EEPROM(unsigned char _address, unsigned char _maxlen, String _text);
		unsigned char read1byte_EEPROM(unsigned char _address);
		char *read_EEPROM(unsigned char _address);
		unsigned char get_pin(String _pin);
		boolean pin_value(String _value);
		void jsonAdd(String *_s, String _key,String _val);		
		void requestAdd(String *_s, String _key,String _val);

};
#endif