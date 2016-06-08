#include <EEPROM.h>
#include <PID_v1.h>
#include <LCD5110_Basic.h>
#include <OneWire.h>
#include <avr/pgmspace.h>
#include "DHT.h"

//      SCK  - Pin 8  17
//      MOSI - Pin 9  16
//      DC   - Pin 10 15
//      RST  - Pin 11 11
//      CS   - Pin 12 14
#define LCD_SCK 17
#define LCD_MOSI  16
#define LCD_DC 15
#define LCD_RST 11
#define LCD_CS 14
#define SSR_PIN 9
#define ONE_WIRE_BUS 2
#define DHTPIN 4
#define LCD_LIGHT 12
#define ALERT_LAMP  13

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
#define MENU_BUTTON 5
#define UP_BUTTON 6
#define DOWN_BUTTON 7
#define LIGHT_BUTTON 8
#define MAX_MENU  2
#define MENU_NORMAL  0
#define MENU_SETTEMP  1
#define MENU_SETHUMI  2
const char lb_menu0[] PROGMEM="Heater chamber";
const char lb_menu1[] PROGMEM="Setup Temp";
const char lb_menu2[] PROGMEM="Setup Humi";
const char* const lb_menu[] PROGMEM ={lb_menu0,lb_menu1,lb_menu2};
#define STEP_SET_TEMP 0.1
#define STEP_SET_HUMI 0.1
byte buttons[] = {MENU_BUTTON, UP_BUTTON, DOWN_BUTTON,LIGHT_BUTTON};
char  NowMenu=0;

#define NUMBUTTONS sizeof(buttons)
#define DEBOUNCE 10
volatile boolean last_state[NUMBUTTONS];
volatile long lasttime;



#define _CONFIGS_ 0 
#define DHTTYPE DHT22   

struct configs{
  boolean debug=true;
  char* serial[10]={"THC0000001"};
  float DispTemp=60;
  float DispHumi=60;
};

configs cfg;
OneWire  ds(ONE_WIRE_BUS);
DHT dht(DHTPIN, DHTTYPE);

double Setpoint, Input, Output, LastInput;
//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;

char row1[14];
String row2="";
String row3="";
String row4="";
String row5="";
String row6="";

PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);
//      SCK  - Pin 8  17
//      MOSI - Pin 9  16
//      DC   - Pin 10 15
//      RST  - Pin 11 11
//      CS   - Pin 12 14
LCD5110 LCD(LCD_SCK,LCD_MOSI,LCD_DC,LCD_RST,LCD_CS);
extern uint8_t arduino_logo[];
extern uint8_t SmallFont[];
void setup(void) {
//  save_configs();
  Serial.begin(115200);
  read_configs();
  // Make input & enable pull-up resistors on switch pins
  for (byte i=0; i<NUMBUTTONS;i++){
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
// Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

//Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;
  LCD.InitLCD();  
  pinMode(LCD_LIGHT, OUTPUT); digitalWrite(LCD_LIGHT, LOW);
  pinMode(ALERT_LAMP, OUTPUT);  digitalWrite(ALERT_LAMP, HIGH);
  pinMode(SSR_PIN, OUTPUT);
  Setpoint=cfg.DispTemp;
  myPID.SetMode(AUTOMATIC);
  dht.begin();
  LCD.drawBitmap(0, 0, arduino_logo, 84, 48); delay(2000);
  LCD.invert(true); delay(500); LCD.invert(false);  delay(500);
  LCD.setFont(SmallFont);
  LCD.clrScr(); delay(500);
  digitalWrite(ALERT_LAMP, LOW);
}
SIGNAL(TIMER2_OVF_vect) { check_buttons();}

void loop(void) {
  strcpy_P(row1, (char*)pgm_read_word(&(lb_menu[NowMenu])));
  LCD.clrScr();
  LCD.print(row1, LEFT, 0);
  LCD.print(row2, LEFT, 8);
  LCD.print(row3, LEFT, 16);
  LCD.print(row4, LEFT, 24);
  LCD.print(row5, LEFT, 32);
  LCD.print(row6, LEFT, 40);
  
  Setpoint=cfg.DispTemp;
  float ndo = read_ds(&row6);
  if(ndo!=9999) Input=ndo;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
  if((NowMenu==MENU_NORMAL)||(NowMenu==MENU_SETTEMP)){
    row2="T: "+String(Setpoint,1)+" "+String(Input,1);
    row4="T read: "+String(t);
  }else{
    row2="";row4="";
  }
  if((NowMenu==MENU_NORMAL)||(NowMenu==MENU_SETHUMI)){
    row3="H: "+String(cfg.DispHumi,1)+" "+String(h,1);  
  }else{
    row3="";
  }
  
  double gap = abs(Setpoint-Input); //distance away from setpoint
  if (gap < 10)
  {  //we're close to setpoint, use conservative tuning parameters
    myPID.SetTunings(consKp, consKi, consKd);
  }
  else
  {
   //we're far from setpoint, use aggressive tuning parameters
   myPID.SetTunings(aggKp, aggKi, aggKd);
  }
  myPID.Compute();
  analogWrite(SSR_PIN, (int)Output);
  row5="Output :"+String(Output);
}
void check_buttons(void){
  byte now_state[NUMBUTTONS];
  digitalWrite(ALERT_LAMP,(boolean)NowMenu);
  if ((lasttime + DEBOUNCE) > millis()) {
    return; // not enough time has passed to debounce
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  for (byte i=0; i<NUMBUTTONS;i++){
    now_state[i]=digitalRead(buttons[i]);   // read the button
    if (now_state[i] != last_state[i]) {
      if (now_state[i] == LOW) {
        //process button
        process_buttons(i);
      }
    last_state[i] = now_state[i];   // keep a running tally of the buttons
    }
  }
};

void process_buttons(byte bt){
    switch (buttons[bt]) {
    case MENU_BUTTON:
      NowMenu++;
      if(NowMenu>MAX_MENU){NowMenu=0;}
      break;
    case UP_BUTTON:
      UpButton(NowMenu);
      break;
    case DOWN_BUTTON:
      DownButton(NowMenu);
      break;
    case LIGHT_BUTTON:
      digitalWrite(LCD_LIGHT, !digitalRead(LCD_LIGHT));
      break;
  } 
};
void UpButton(byte mn){
    switch (mn) {
    case MENU_SETTEMP:
      set_temp(cfg.DispTemp + STEP_SET_TEMP);
      break;
    case MENU_SETHUMI:
      set_humi(cfg.DispHumi + STEP_SET_HUMI);
      break;
  } 
};
void DownButton(byte mn){
    switch (mn) {
    case MENU_SETTEMP:
      set_temp(cfg.DispTemp - STEP_SET_TEMP);
      break;
    case MENU_SETHUMI:
      set_humi(cfg.DispTemp - STEP_SET_HUMI);
      break;
  } 
};
void read_configs(void){
  EEPROM.get( _CONFIGS_, cfg );
};
void save_configs(void){
  EEPROM.put( _CONFIGS_, cfg );
};

void set_debug(boolean _debug){
  cfg.debug=_debug;
  save_configs();
  read_configs();
};
boolean set_serial(String _serial){
  if(_serial.length()==9){
    _serial.toCharArray(*cfg.serial, _serial.length()+1);
    save_configs();
    read_configs();
    return true;
  }else{
    return false;
  }
};
void set_temp(float _temp){
  cfg.DispTemp = _temp;
  save_configs();
  read_configs();
};
void set_humi(float _humi){
  cfg.DispHumi = _humi;
  save_configs();
  read_configs();
};
// Analysis command : key==value\n

// == separate
// "\n" end
void DeCommand(String *_request, String _separate,String *_key,String *_val){
    int g_end,g_compare;
    g_compare = _request->indexOf(_separate,0);
    g_end = _request->indexOf('\n',g_compare);
  *_key=_request->substring(0, g_compare);
    *_val=_request->substring(g_compare+_separate.length(), g_end);
}
float read_ds(String *stt){
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8];
  boolean update_temp=0;
  if ( !ds.search(addr)) {
    *stt="Scanning sensor";
    ds.reset_search();
    delay(250);
    return 9999;
  }
  
  if (OneWire::crc8(addr, 7) != addr[7]) {
    *stt="CRC is not valid!";
    return 9999;
  }
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      *stt="Chip = DS18S20 ";  // or old DS1820
      type_s = 1;
      update_temp=1;
      break;
    case 0x28:
      *stt="Chip = DS18B20 ";
      type_s = 0;
      update_temp=1;
      break;
    case 0x22:
      *stt="Chip = DS1822 ";
      type_s = 0;
      update_temp=1;
      break;
    default:
      *stt="Not DS18x20 device.";
      update_temp=0;
  } 

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);        // start conversion, with parasite power on at the end
  
  delay(1000);     // maybe 750ms is enough, maybe not
  // we might do a ds.depower() here, but the reset will take care of it.
  
  present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE);         // Read Scratchpad
  for ( i = 0; i < 9; i++) {           // we need 9 bytes
    data[i] = ds.read();
  }
  // Convert the data to actual temperature
  // because the result is a 16 bit signed integer, it should
  // be stored to an "int16_t" type, which is always 16 bits
  // even when compiled on a 32 bit processor.
  int16_t raw = (data[1] << 8) | data[0];
  if (type_s&&update_temp) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else if((!type_s)&&update_temp) {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  return (double)raw / 16.0;
};
