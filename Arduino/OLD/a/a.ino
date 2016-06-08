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
struct request {
  String command;
  String serial;
  String key;
  String ssid;
  String password;
  String ip;
  unsigned char pin;
  unsigned char value;
};
// write change to EEPROM
boolean update_EEPROM(unsigned char addr, unsigned char maxlen, String text) {
  // leghth of array (=leghth of string +1) will be write to EEPROM
  // +1 because: end of array is "\0"
  char len = text.length() + 1;
  // Init an array
  char arr[len];
  // Convert string to array
  text.toCharArray(arr, len);
  // Write array to EEPROM when leghth of array <= max leghth in EEPROM
  if (len <= maxlen) {
    // Write leghth of array in fisrt byte of block content in EEPROM
    EEPROM.write(addr, len);
    for (unsigned char i = 0; i < len; i++) {
      // Write byte by byte, start from second byte of block content in EEPROM
      EEPROM.write(i + addr + 1, arr[i]);
    }
    // Save from RAM to EEPROM
    EEPROM.commit();
    // Return true when leghth of array <= max leghth in EEPROM
    return true;
  } else {
    // Return false when leghth of array > max leghth in EEPROM and not save to EEPROM
    return false;
  }
}
char *read_EEPROM(unsigned char addr) {
  // Read leghth of array in fisrt byte of block content in EEPROM
  char len = EEPROM.read(addr);
  char *re;
  re = (char*)malloc(len);
  for (unsigned char i = 0; i < len; i++) {
    // Read byte to byte, start from second byte of block content in EEPROM
    re[i] = EEPROM.read(i + addr + 1);
  }
  // return array of result
  return re;
}
// Init global variables
String __serial = "A10000";
String __st_key = "12345";
String stringOne = "GET /?c=setup&&ssid=trungthanh&&pass=12112007 HTTP/1.1\r\nHost: 192.168.4.1\r\nConnection: close\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,";
configs rom;

// Create wifi server
WiFiServer server(80);
void setup() {
  // Open EEPROM with size 512 byte
  EEPROM.begin(512);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
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
  // Read serial of device
  char *a = read_EEPROM(__serial_adrr);
  rom.serial = a;
  free(a);
  // Read key to connect to device
  char *b = read_EEPROM(__key_adrr);
  rom.key = b;
  free(b);
  // Read ssid of wifi
  char *c = read_EEPROM(__ssid_adrr);
  rom.ssid = c;
  free(c);
  // Read password of wifi
  char *d = read_EEPROM(__password_adrr);
  rom.password = d;
  free(d);
  // Read IP of device
  char *e = read_EEPROM(__IP_adrr);
  rom.ip = e;
  free(e);
  // convert string of ssid, password to array (as update_EEPROM)
  char str_len = rom.ssid.length();
  unsigned char c_ssid[str_len];
  rom.ssid.toCharArray(c_ssid, str_len);

  str_len = rom.password.length();
  unsigned char c_password[str_len];
  rom.password.toCharArray(c_password, str_len);

  str_len = rom.ip.length();
  unsigned char c_ip[str_len];
  rom.ip.toCharArray(c_ip, str_len);

  // Connect to wifi
  Serial.print("Connecting to ");
  Serial.println(c_ssid);

  WiFi.begin(c_ssid, c_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Setup static IP
  IPAddress ip(c_ip[0], c_ip[1], c_ip[2], c_ip[3]);
  WiFi.config(ip);
  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  Serial.println(WiFi.localIP());
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
      delay(1);
    }
    // Read the first line of the request
    String req = client.readStringUntil('\r');
    Serial.println(req);
    client.flush();
    // Init configs struct
    request rq[5];
    // get GET the request
    // indexOf(string, start poistion) returns the position (i.e. index) of a particular character
    int start1 = stringOne.indexOf('/');
    int compare1 = stringOne.indexOf('=', start1);
    int start2 = stringOne.indexOf('&', compare1);
    int compare2 = stringOne.indexOf('=', start2);
    int start3 = stringOne.indexOf('&', compare2);
    int compare3 = stringOne.indexOf('=', start3);
    int ends = stringOne.indexOf(' ', compare3);
    rq.command = req.substring(compare1 + 1, start2);


  };
  // indexOf(string, start poistion) returns the position (i.e. index) of a particular character
  // in a string. For example, if you were parsing HTML tags, you could use it:
  int start1 = stringOne.indexOf('/');
  int compare1 = stringOne.indexOf('=', start1);
  int start2 = stringOne.indexOf('&', compare1);
  int compare2 = stringOne.indexOf('=', start2);
  int start3 = stringOne.indexOf('&', compare2);
  int compare3 = stringOne.indexOf('=', start3);
  int ends = stringOne.indexOf(' ', compare3);
  Serial.println("\nThe GET " + stringOne.substring(start1 + 2, compare1) + " : " + stringOne.substring(compare1 + 1, start2));
  Serial.println("\nThe GET " + stringOne.substring(start2 + 2, compare2) + " : " + stringOne.substring(compare2 + 1, start3));
  Serial.println("\nThe GET " + stringOne.substring(start3 + 2, compare3) + " : " + stringOne.substring(compare3 + 1, ends));
  String c = stringOne.substring(compare1 + 1, start2);
  String ssid = stringOne.substring(compare2 + 1, start3);
  String password = stringOne.substring(compare3 + 1, ends);
  update_EEPROM(__ssid_adrr, __ssid_max, ssid);
  update_EEPROM(__password_adrr, __password_max, password);

  Serial.println("\nThe EEPROM serial: " + rom.serial);
  Serial.println("\nThe EEPROM key: " + rom.key);
  Serial.println("\nThe EEPROM ssid: " + rom.ssid);
  Serial.println("\nThe EEPROM password: " + rom.password);
  Serial.println("\nThe string: " + stringOne);
  // do nothing while true:

}



