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

#define WifiIPAddr	10	// 10-21	4byte IP, 4byte Mask, 4byte gateway
#define	WifiInfoAddr	22	// 22-101	40byte SSID, 40byte password
#define	DeviceInfoAddr	102	// 102-131	10byte serial, 10byte key, 10byte adminkey

#define __first_start 0		// Lan khoi dong dau tien
#define __ip_static 1		// Chon setup static IP cho thiet bi
#define __debug 2		// Chon setup static IP cho thiet bi
#define __SERIAL 10	// Block dia chi Serial thiet bi 10byte, 1 byte chieu dai, 8 byte noi dung, 1 byte \0

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

struct WifiIP{
	IPAddress	ip;
	IPAddress	mask;
	IPAddress	gateway;
}
struct	WifiInfo{
	char	ssid[40];
	char	password[40];
}
struct	DeviceInfo{
	char	serial[10];
	char	key[10];
	char	admin[10];
}


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
		void _GET(String *_val, String *_request,String _key)
	private:
		WifiIP	ip;
		WifiInfo	wifi;
		DeviceInfo	device;
		
		String http_header_info="\r\nConnection: close\r\nCache-Control: max-age=0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nUser-Agent: wifi-switch\r\nAccept-Encoding: gzip, deflate, sdch\r\nAccept-Language: vi-VN,vi;q=0.8,fr-FR;q=0.6,fr;q=0.4,en-US;q=0.2,en;q=0.2\r\n\r\n";
		String http_header_host=" HTTP/1.1\r\nHost: ";
		String http_header_respone="HTTP/1.1 200 OK\r\nAccess-Control-Allow-Origin: *\r\nContent-Type: application/json\r\nUser-Agent: Wifi-switch\r\nConnection: close\r\n\r\n";
};
#endif