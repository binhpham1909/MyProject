#ifndef WifiBinh_h
#define WifiBinh_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define WifiIPAddr	10	// 10-21	4byte IP, 4byte Mask, 4byte gateway
#define	WifiInfoAddr	22	// 22-101	40byte SSID, 40byte password
#define	DeviceInfoAddr	102	// 102-131	10byte serial, 10byte key, 10byte adminkey


struct WifiIP{
	IPAddress	ip;
	IPAddress	mask;
	IPAddress	gateway;
};
struct	WifiInfo{
	char	ssid[40];
	char	password[40];
};
struct	DeviceInfo{
	char	serial[10];
	char	key[10];
	char	admin[10];
};

class ESPHB	// class chua cac ham xu ly cua thu vien
{
	public:	
		void _GET(String *_val, String *_request,String _key);
//		void StoreInit(int _size);
		void Init(void);
		void StoreIP(String _ip,String _mask,String _gateway);
		void GetIP(String *_s);
		void GetMask(String *_s);
		void GetGateway(String *_s);
		void GetSsid(String *_s);
		void GetPassword(String *_s);
		void GetSerial(String *_s);
		void GetKey(String *_s);
		void GetAdmin(String *_s);
	private:
		WifiIP	ip;
		WifiInfo	wifi;
		DeviceInfo	device;
		void IPtoString(IPAddress _ip,String * _s);
		void StringtoIP(String *_s,IPAddress *_ip);
};
#endif