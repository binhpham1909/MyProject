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
// EEPROM byte:
// 0: bien khoi dong - 0: khoi dong lan dau; 1: khoi dong lan sau
// x0: so ky tu trong chuoi
// 10-19: num byte of SERIAL  (max 9)
// 20-29: num byte of key  (max 9)
// 30-69: num byte of ssid  (max 39)
// 70-109: num byte of password (max 39)
// arrtostr: String var; ga'n: var=arr
boolean update_EEPROM(unsigned char addr, unsigned char maxlen, char arr[]) {
  char len = sizeof(arr)-1;
  if (len <= maxlen) {
    EEPROM.write(addr, len);
    for (unsigned char i = 0; i < len; i++) {
      EEPROM.write(i + addr + 1, arr[i]);
      EEPROM.commit();
    }
    return true;
  } else {
    return false;
  }
}
char *read_EEPROM(unsigned char addr) {
  char len = EEPROM.read(addr);
  char *re;
  re=(char*)malloc(len);
  for (unsigned char i = 0; i < len; i++) {
    re[i] = EEPROM.read(i + addr + 1);
  }
  return re;
}
// khai bao bien toan cuc
char *serial,*key,*ssid,*password;
char st_start;
char st_key[]="12345";
char st_serial[]="A10000";
//String serial="A10000";
void setup() {
   EEPROM.begin(512);
   
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for Leonardo only
  }
  // send an intro:
  Serial.println("\n Test string functions:");
  Serial.println();
  char st_start = EEPROM.read(__first_start);
  char *a = read_EEPROM(__serial_adrr);
  serial=a;
  free(a);
  char *b = read_EEPROM(__key_adrr);
  key=b;
  free(b);
  char *c = read_EEPROM(__ssid_adrr);
  ssid=c;
  free(c);
  char *d = read_EEPROM(__password_adrr);
  password=d;
  free(d);
  if(st_start!=1){  // write serial
    update_EEPROM(__serial_adrr, __serial_max, st_serial);
    update_EEPROM(__key_adrr, __key_max, st_key);
    Serial.println(serial);
    EEPROM.write(__first_start, 1);
    EEPROM.commit();
  }
}

void loop() {
  // indexOf(string, start poistion) returns the position (i.e. index) of a particular character
  // in a string. For example, if you were parsing HTML tags, you could use it:
  String stringOne = "GET /?c=setup&&ssid=trungthanh&&pass=12112007 HTTP/1.1\r\nHost: 192.168.4.1\r\nConnection: close\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,";
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
  String n_ssid = stringOne.substring(compare2 + 1, start3);
  char len1 = n_ssid.length() + 1;
  char arr[len1];
  n_ssid.toCharArray(ssid, len1);
  String n_password = stringOne.substring(compare3 + 1, ends);
    update_EEPROM(__ssid_adrr, __ssid_max, ssid);
    update_EEPROM(__password_adrr, __password_max, password);
    WiFi.begin(arr, arr);
  Serial.println(st_start);  
  Serial.println(serial);
  Serial.println(key);
  Serial.println(ssid);
  Serial.println(password);
  Serial.println("\nThe string: " + n_ssid);
  Serial.println("\nThe string: " + n_password);
  Serial.println("\nThe string: " + stringOne);
  // do nothing while true:
  while (true) {
    //
  };
}


