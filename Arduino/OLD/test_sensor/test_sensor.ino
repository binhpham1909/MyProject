#include <WifiBinh.h>


ESPHB ESP;
WifiIP Ip;
Ip.ip=(192,168,1,2);
Ip.mask=(255,255,255,0);
Ip.gateway=(192,168,1,1);
WifiInfo  wfinfo={"TrungThanh","12112007"};
DeviceInfo  dvinfo={"a2345","12345","67890"};
ESP.Init();

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  String stringOne;
  // send an intro:
  Serial.println("\n\nString indexOf() and lastIndexOf()  functions:");
  Serial.println();
  EEPROM.put(WifiIPAddr,Ip);
  EEPROM.put(WifiInfoAddr,wfinfo);
  EEPROM.put(DeviceInfoAddr,dvinfo);
}

void loop() {
  String val;
  String stringOne = "GET /?command=setup&&ssid=trungthanh&&pass=12112007 HTTP/1.1\r\nHost: 192.168.4.1\r\nConnection: close\r\n";
  ESP._GET(&val,&stringOne,"command");
  Serial.println("GET  command is " + val);
  ESP._GET(&val,&stringOne,"ssid");
  Serial.println("GET  ssid is " + val);
  ESP._GET(&val,&stringOne,"pass");
  Serial.println("GET  password is " + val);
  ESP._GET(&val,&stringOne,"assa");
  Serial.println("GET  assa is " + val);
  // do nothing while true:
  while(true);
}
