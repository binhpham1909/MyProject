/*
 * EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it
 * to the computer.
 * This example code is in the public domain.
 */

#include <EEPROM.h>
#define MAX_SERIAL_LEN  10  // Serial of device 12         Bytes 20 to 31
#define MAX_SSID_LEN  48
#define MAX_PASSWORD_LEN  32 // Key to control 12           Bytes 32 to 43
#define MAX_STATICIP_LEN  10
#define MAX_KEY_LEN 10 // SSID Wifi 34 byte           Bytes 44 to 77
#define MAX_ADMIN_LEN 10
#define MAX_SERVER_LEN  48 // PASSWORD Wifi 34 byte   Bytes 78 to 111
// start reading from the first byte (address 0) of the EEPROM
int address = 0;
byte value;
struct EEPROM_storage {
  uint8_t DEBUG;
  uint8_t FIRST_START;
  uint8_t IS_STATICIP;
  uint8_t WIFI_CONN_TIMEOUT;
 
  char DEVICE_SERIAL[MAX_SERIAL_LEN + 1]; // WIFI ssid + null
  char WF_SSID[MAX_SSID_LEN + 1]; // WIFI ssid + null
  char PASSWORD[MAX_PASSWORD_LEN + 1]; // WiFi password,  if empyt use OPEN, else use AUTO (WEP/WPA/WPA2) + null
  char STATICIP[MAX_STATICIP_LEN + 1]; // staticIP, if empty use DHCP + null
  char KEY[MAX_KEY_LEN + 1]; // WIFI ssid + null
  char ADMIN[MAX_ADMIN_LEN + 1]; // WIFI ssid + null
  char SERVER[MAX_SERVER_LEN + 1]; // WIFI ssid + null
} STORAGE;
String stringto="motconvit";
template <class T> void EEPROMRead(int StartAddress,T *storageVar){
  uint8_t * bytesToReadEEPROM = (uint8_t *)storageVar;
  const int STORAGE_SIZE = sizeof(*storageVar);
  for (int i = 0; i < STORAGE_SIZE; i++) {
    bytesToReadEEPROM[i] = EEPROM.read(StartAddress + i);
  }
};
template <class T> void EEPROMWrite(int StartAddress,T *storageVar){
  uint8_t * bytesToWriteEEPROM = (uint8_t *)storageVar;
  const int STORAGE_SIZE = sizeof(*storageVar);
  for (int i = 0; i < STORAGE_SIZE; i++) {
    EEPROM.write(StartAddress + i,bytesToWriteEEPROM[i]);
  }
};
void StringToArray(char* arrayTo, String *StringFrom,int maxlen){
    int _len=StringFrom->length();
    if(maxlen>_len){
        StringFrom->toCharArray(arrayTo,_len+1);
    }
};
void setup() {
  // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 /*
  char *a="abcdef";
  STORAGE.DEVICE_SERIAL[0]=a[0];
  STORAGE.DEVICE_SERIAL[1]=a[1];
  STORAGE.DEVICE_SERIAL[2]=a[2];
  STORAGE.DEVICE_SERIAL[3]=a[3];
  STORAGE.DEVICE_SERIAL[4]=a[4];
  STORAGE.DEVICE_SERIAL[5]=a[5];
  EEPROMWrite(0,&STORAGE);*/
  
}

void loop() {
  EEPROMRead(0,&STORAGE);
  Serial.println(STORAGE.DEVICE_SERIAL);
  StringToArray(STORAGE.DEVICE_SERIAL, &stringto,MAX_SERIAL_LEN);
  Serial.println(STORAGE.DEVICE_SERIAL);
  while(true){};
}
