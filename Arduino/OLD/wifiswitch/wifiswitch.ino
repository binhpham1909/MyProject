/*
  String indexOf() and lastIndexOf() functions

 Examples of how to evaluate, look for, and replace characters in a String

 created 27 July 2010
 modified 2 Apr 2012
 by Tom Igoe

 http://arduino.cc/en/Tutorial/StringIndexOf

 This example code is in the public domain.
 */
// String to array:
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#define __first_start 0
#define __serial_adrr 10
#define __serial_max 9
#define __key_adrr 20
#define __key_max 9
#define __ssid_adrr 30
#define __ssid_max 39
#define __password_adrr 70
#define __password_max 39
#define __IP_adrr 110
#define __IP_max 4
#define _pin1 0
#define _pin2 2
#define _pin3 16
// EEPROM byte:
// 0: bien khoi dong - 0: khoi dong lan dau; 1: khoi dong lan sau
// x0: so ky tu trong chuoi
// 10-19: num byte of SERIAL  (max 9)
// 20-29: num byte of key  (max 9)
// 30-69: num byte of ssid  (max 39)
// 70-109: num byte of password (max 39)
// arrtostr: String var; ga'n: var=arr
struct configs {
  unsigned char st_start;
  String serial;
  String key;
  String ssid;
  String password;
  String ip;
};
struct http_read {
  String names;
  String value;
};
struct request{
  String command;
  String key;
  String newkey;
  String ssid;
  String password;
  String ip;
  String pin;
  String value;
};
// write change to EEPROM
boolean update_EEPROM(unsigned char addr, unsigned char maxlen, String text){
  char len = text.length() + 1;  // leghth of array (=leghth of string +1) will be write to EEPROM; +1 because: end of array is "\0"
  char arr[len];  // Init an array
  text.toCharArray(arr, len);  // Convert string to array
  if (len <= maxlen) {  // Write array to EEPROM when leghth of array <= max leghth in EEPROM
    EEPROM.write(addr, len);  // Write leghth of array in fisrt byte of block content in EEPROM
    for (unsigned char i = 0; i < len; i++) {
      EEPROM.write(i + addr + 1, arr[i]);  // Write byte by byte, start from second byte of block content in EEPROM
    }
    EEPROM.commit();// Save from RAM to EEPROM
    return true;  // Return true when leghth of array <= max leghth in EEPROM
  } else {
    return false;  // Return false when leghth of array > max leghth in EEPROM and not save to EEPROM
  }
}
char *read_EEPROM(unsigned char addr) {
  char len = EEPROM.read(addr);  // Read leghth of array in fisrt byte of block content in EEPROM
  char *re;
  re = (char*)malloc(len);
  for (unsigned char i = 0; i < len; i++) {
    re[i] = EEPROM.read(i + addr + 1);  // Read byte to byte, start from second byte of block content in EEPROM
  }
  return re;  // return array of result
}
void toggle_pin(String pinname, String value){
  boolean toggle;
  if(value=="ON"){toggle=HIGH;}else{toggle=LOW;};
  if(pinname=="pin1"){digitalWrite(_pin1, toggle);}
  else if(pinname=="pin2"){digitalWrite(_pin2, toggle);}
  else if(pinname=="pin3"){digitalWrite(_pin3, toggle);}
}
// Init global variables
String __serial = "A10000";
String __st_key = "12345";
String stringOne = "GET /?c=setup&&ssid=trungthanh&&pass=12112007 HTTP/1.1\r\nHost: 192.168.4.1\r\nConnection: close\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,";
configs rom;
request _get;
unsigned char timeout=0;
const char *assid = "Binh";
const char *apassword = "12345678";
const char* ss = "TrungThanh";
const char* pp = "12112007";


// Create wifi server
WiFiServer server(80);
void setup() {
// Open EEPROM with size 512 byte
  EEPROM.begin(512);
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
  // wait for serial port to connect. Needed for Leonardo only
  }
// send an wellcome messaage:
  Serial.println("\n Wellcome to ESP wifi switch control.");
  Serial.println("Read EEPROM");
// read fist startup to config serial and first key connect to EEPROM
// 9 is not fist startup
  rom.st_start = EEPROM.read(__first_start);
  if (rom.st_start != 9) { // write serial
    update_EEPROM(__serial_adrr, __serial_max, __serial);
    update_EEPROM(__key_adrr, __key_max, __st_key);
    Serial.println("\nUpdated: serial - " + __serial + "key: " + __st_key);
    EEPROM.write(__first_start, 9);
    EEPROM.commit();
  }
  char *a = read_EEPROM(__serial_adrr);  rom.serial = a;  free(a);  // Read serial of device
  char *b = read_EEPROM(__key_adrr);  rom.key = b;  free(b);  // Read key to connect to device
  char *c = read_EEPROM(__ssid_adrr);  rom.ssid = c;  free(c);  // Read ssid of wifi
  char *d = read_EEPROM(__password_adrr);  rom.password = d;  free(d);  // Read password of wifi
  char *e = read_EEPROM(__IP_adrr);  rom.ip = e;  free(e);  // Read IP of device
// convert string of ssid, password to array (as update_EEPROM)
  char str_len = rom.ssid.length()+1;
  char c_ssid[str_len];
  rom.ssid.toCharArray(c_ssid, str_len);
  str_len = rom.password.length()+1;
  char c_password[str_len];
  rom.password.toCharArray(c_password, str_len);

  str_len = rom.ip.length()+1;
  char c_ip[str_len];
  rom.ip.toCharArray(c_ip, str_len);
  WiFi.mode(WIFI_STA);
  Serial.print("Connecting to ");
  Serial.println(c_ssid);
  Serial.println(c_password);
  //WiFi.begin(c_ssid, c_password);    // Connect to wifi
  WiFi.begin(c_ssid, c_password);    // Connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");timeout++;
    if(timeout>60){
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP(assid, apassword);
      Serial.println("Cannot connected, you can connect to:");
      Serial.println(WiFi.softAPIP());
      break;}
    }
  Serial.println("");
  Serial.println("WiFi connected");
//  IPAddress ip(c_ip[0], c_ip[1], c_ip[2], c_ip[3]);  // Setup static IP
//  WiFi.config(ip);
  server.begin();    // Start the server
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  // Print the IP addressWiFi.softAPIP()
}

void loop() {
  while (true) {
    // Check if a client has connected
    WiFiClient client = server.available();
    if (!client) {
      return;
    }
// Wait until the client sends some data
    Serial.println("new client");
    while (!client.available()) {
      // process main until client receive request
      delay(1);
      
    }
    // Read the first line of the request
    String req = client.readStringUntil('\r');
    client.flush();
    
    Serial.println(req);
    http_read rq[5];  // Init configs struct
// get GET the request
// indexOf(string, start poistion) returns the position (i.e. index) of a particular character
// GET /?c=setup&&ssid=trungthanh&&pass=12112007 HTTP/1.1\r\nHost: 192.168.4.1\r\nConnection: close\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,
    unsigned char index=0;
    int start = req.indexOf('/');
    int compare = 0;
    int ends = stringOne.indexOf(' ', compare);
    while(start>0){
      compare = req.indexOf('=', start);
      rq[index].names=req.substring(start+2, compare);
      start = req.indexOf('&', compare);
      if(start<0){
        rq[index].value=req.substring(compare+1, ends);
        Serial.println(rq[index].value);
        }else{ rq[index].value=req.substring(compare+1, start);}
      index++;
    };
    // process request
    for(unsigned char i=0;i<=index;i++){
      if(rq[i].names=="command"){_get.command=rq[i].value;}
        else if(rq[i].names=="key"){_get.key=rq[i].value;}
        else if(rq[i].names=="newkey"){_get.newkey=rq[i].value;}
        else if(rq[i].names=="ssid"){_get.ssid=rq[i].value;}
        else if(rq[i].names=="password"){_get.password=rq[i].value;}
        else if(rq[i].names=="ip"){_get.ip=rq[i].value;}
        else if(rq[i].names=="pin"){_get.pin=rq[i].value;}
        else if(rq[i].names=="value"){_get.value=rq[i].value;}
    };
    if(_get.key==rom.key){
      if(_get.command=="control"){
        update_EEPROM(__key_adrr, __key_max, _get.newkey);
      }
      else if((_get.command=="wifi")&&(_get.ssid!="")){
        update_EEPROM(__ssid_adrr, __ssid_max, _get.ssid);
        update_EEPROM(__password_adrr, __password_max, _get.password);
      }
      else if((_get.command=="key")&&(_get.newkey!="")){
        update_EEPROM(__key_adrr, __key_max, _get.newkey);
      };
    };
    req="";
  };
}




