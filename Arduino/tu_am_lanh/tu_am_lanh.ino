#include <EEPROM.h>


#include <avr/pgmspace.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <Wire.h> 
#include <DallasTemperature.h>
// PIN connector
// LCD 1602
LiquidCrystal_I2C LCD(0x20,16,2);  // set the LCD address to 0x20 for a 16 chars and 2 line display
// Relay pin
#define BAT(X) digitalWrite(X, LOW)  
#define TAT(X) digitalWrite(X, HIGH)  
#define HEATER_PIN 5
#define COOLER_PIN 6
#define COOLER_VAL_PIN 7
#define FAN_PIN 7
#define BELL_PIN 7
// Sensor pin
#define DS18B20_PIN A3

// Rotaty Encoder PIN
#define A_PIN 9
#define B_PIN 8
#define BTN_PIN 7

// Text display menu
#define MAX_SUBMENU  2
//const char lb_menu0[] PROGMEM="Heater control";
//const char lb_menu1[] PROGMEM="Set Temp";
///const char lb_menu2[] PROGMEM="Mode run";
//const char lb_menu3[] PROGMEM="Timer";
//const char* const lb_menu[] PROGMEM ={lb_menu0, lb_menu1, lb_menu2, lb_menu3};
#define _BLINK_TIME_    500 
unsigned int lastBlink;
boolean lastBlinkState = false;
// Menu state
boolean inSetup = false;
boolean inSubMenu = false;
boolean inSubItem = false;
// Menu position
byte posSubMenu = 0;
byte posSubItem = 0;
int posMenuCurent = 0;
// Menu array
byte maxSubItem[MAX_SUBMENU];

char row1[2][7];
char row2[2][7];
// DS18B20
OneWire oneWire(DS18B20_PIN);
DallasTemperature sensors(&oneWire);
DeviceAddress inside = { 0x28, 0x9A, 0x80, 0x40, 0x04, 0x00, 0x00, 0xD5 }; // "1"
DeviceAddress outside = { 0x28, 0xE1, 0xC7, 0x40, 0x04, 0x00, 0x00, 0x0D }; // "2"
float inTemp, outTemp, lastInTemp;

// EEPROM
#define _CONFIGS_ 0   
#define _CALIB_IN_ 10 
#define _CALIB_OUT_ 55 
struct configs{
  byte mode;
  float setTemp;
  float timer;
} cfg, ncfg;
struct calibration{
  float t0;
  float t10;
  float t20;
  float t30;
  float t40;
  float t50;
  float t60;
  float t70;
  float t80;
  float t90;
  float t100;
}  cal,ncal;
// Encoder
ClickEncoder *encoder;
int16_t lastEn, valueEn;


// control
unsigned int timerRemain;   // thoi gian timer con lai

boolean pumpNow = false;
boolean lastTimerState = false;
boolean pumpNowAuto, pumpNowMan, pumpNowTimer, pumpAllow;

// Serial
String serialInput="";
boolean serialComplete = false;
void setup(void) {
    Serial.begin(115200);
    // 2 line enable at first setup
    //cfg = {0, 1023, 0, 40, 5, 5, 120, 14, 0, 11, 0};
    //EEPROM.put( _CONFIGS_, cfg );
    EEPROM.get( _CONFIGS_, cfg );
    initGPIO();
    initLCD();
    initECRotaty();
    initDS18B20();
    createMenuList();
}

void loop(void) {
    readTemp();
    processEncoder();
    processEncoderBtn();
    updateLCD();
    displayLCD();
    control();
    processSerial(serialInput);
}

void serialEvent(){
    while (!serialComplete&&Serial.available()) {
        // get the new byte:
        char inChar = (char)Serial.read();
        // add it to the inputString:
        serialInput += inChar;
        // if the incoming character is a newline, set a flag
        // so the main loop can do something about it:
        if (inChar == '\n') {
            serialComplete = true;
        }
    }
};
void processSerial(String& input){
    if(!serialComplete)  return;
    int startS = input.indexOf('>');
    int sepS = input.indexOf(':');
    int endS = input.indexOf('\n');
    char br = '"';
    char se = ':';
    char ne = ',';
    char op = '{';
    char cl = '}';
    String comma = input.substring(startS+1,sepS);
    String val = input.substring(sepS + 1, endS);
    int comm = comma.toInt();
//    Serial.println(comm);
//    Serial.println(val);
    if(comm == 1){
        Serial.print(op);
        Serial.print(br);   Serial.print(F("mode"));   Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.mode);   Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("adc0"));   Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.adc0);   Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("adc100")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.adc100); Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("moisture")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.moisSet); Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("moistureOffset")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.moisOffset); Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("timerOn")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.timerOn); Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("timerOff")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.timerOff); Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("timeEnable")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.HHEnable); Serial.print('-');   Serial.print(cfg.MMEnable); Serial.print(br); Serial.print(ne);
        Serial.print(br);   Serial.print(F("timeEnable")); Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.HHDisable); Serial.print('-');   Serial.print(cfg.MMDisable); Serial.print(br);
        Serial.println(cl);
    }else if(comm == 11){
        ncfg.mode = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg );
        Serial.println(F("OK"));
    }else if(comm == 12){
        ncfg.timerOn = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 13){
        ncfg.timerOff = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 14){
        ncfg.adc0 = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 15){
        ncfg.adc100 = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 16){
        ncfg.moisSet = val.toFloat();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 17){
        ncfg.moisOffset = val.toFloat();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 18){
        ncfg.HHEnable = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 19){
        ncfg.MMEnable = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 20){
        ncfg.HHDisable = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 21){
        ncfg.MMDisable = val.toInt();
        EEPROM.put( _CONFIGS_, ncfg );
        EEPROM.get( _CONFIGS_, cfg ); 
        Serial.println(F("OK"));
    }else if(comm == 22){
        nTime = t_now;
        nTime.Day = val.toInt();
        RTC.write(nTime);
        Serial.println(F("OK"));
    }else if(comm == 23){
        nTime = t_now;
        nTime.Month = val.toInt();
        RTC.write(nTime);
        Serial.println(F("OK"));
    }else if(comm == 24){
        nTime = t_now;
        nTime.Year = val.toInt();
        RTC.write(nTime);
        Serial.println(F("OK"));
    }else if(comm == 25){
        nTime = t_now;
        nTime.Hour = val.toInt();
        RTC.write(nTime);
        Serial.println(F("OK"));
    }else if(comm == 26){
        nTime = t_now;
        nTime.Minute = val.toInt();
        RTC.write(nTime);
        Serial.println(F("OK"));
    }else if(comm == 27){
        nTime = t_now;
        nTime.Second = val.toInt();
        RTC.write(nTime);
        Serial.println(F("OK"));
    }
    input = "";
    serialComplete = false;
};
// Sub program
void initLCD(){
    LCD.clear();  // Reset the display  
    LCD.home();
    LCD.backlight();  //Backlight ON if under program control  
    LCD.setCursor(8,0); //Start at character 0 on line 0
    LCD.print(F("HBInvent"));
    delay(1000);
    LCD.clear();
};

void initECRotaty(){
    encoder = new ClickEncoder(A_PIN,B_PIN,BTN_PIN,4);
    encoder->setAccelerationEnabled(true);
    Timer1.initialize(1000);    // 1000 microsecond
    Timer1.attachInterrupt(timerIsr);
    lastEn = -1;    
};
void timerIsr() {
    encoder->service();
    lastBlink +=1;
    if(lastBlink > _BLINK_TIME_){
        lastBlink =0;
        lastBlinkState=!lastBlinkState;
    }
};

void initGPIO(){
    pinMode(HEATER_PIN, OUTPUT);  TAT(LIGHT_PIN);
    pinMode(COOLER_PIN, OUTPUT); TAT(PUMP_PIN); 
    pinMode(COOLER_VAL_PIN, OUTPUT); TAT(PUMP_PIN);  
    pinMode(FAN_PIN, OUTPUT); BAT(PUMP_PIN);    
    pinMode(BELL_PIN, OUTPUT); BAT(BELL_PIN);   
};
/// Menu with encoder rotaty - MVC
void initDS18B20(){
    sensors.begin();
    sensors.setResolution(inside, 10);
    sensors.setResolution(outside, 10);
};
void readTemp(){
    float tempC1 = sensors.getTempC(inside);
    float tempC2 = sensors.getTempC(outside);
    if (tempC1 == -127.00){ // Measurement failed or no device found
        LCD.print("Temperature Error");
    }else{
        inTemp = tempC1;
    };
    if (tempC2 == -127.00){ // Measurement failed or no device found
        LCD.print("Temperature Error");
    }else{
        outTemp = tempC2;
    }
};

void control(){
    if(cfg.mode == 0){  // normal
        controlTemp(true);
    }else if(cfg.mode == 1){
    
    }
}
// Control temp
void controlTemp(boolean enable){
    if(enable){// Normal
        if(cfg.setTemp < outTemp){  // che do lam mat
            TAT(HEATER_PIN);    // tat phan lam nong
            if(inTemp > cfg.setTemp){   // neu chua mat
                BAT(COOLER_VAL_PIN);    // mo van lam mat
            }else{  // neu mat roi
                TAT(COOLER_VAL_PIN);    // tat van lam mat
            }
        }else{  // che do lam nong
            TAT(COOLER_PIN);    // tat phan lam mat
            if(inTemp > cfg.setTemp){   // neu qua nong
                TAT(HEATER_PIN);    // tat nhiet
            }else{  // neu chua du nong
                BAT(HEATER_PIN);    // cap nhiet
            }
        }
    }else if(cfg.mode == 1){
        TAT(HEATER_PIN);    // tat phan lam nong
        TAT(COOLER_PIN);    // tat phan lam mat
    }
};

void saveItem(){
    switch(posSubMenu){
            case 1:
            case 2:
            case 3:
                EEPROM.put( _CONFIGS_, ncfg );
                EEPROM.get( _CONFIGS_, cfg );              
                break;
            default: break;
    };
};
// View
void displayLCD(){
    LCD.clear();
    LCD.setCursor(0,0); LCD.print(row1[0]);  LCD.setCursor(7,0);  LCD.print(": ");   LCD.setCursor(9,0);    LCD.print(row1[1]);    LCD.setCursor(14,0);    LCD.print(223, BIN);    LCD.setCursor(15,0);    LCD.print("C");
    LCD.setCursor(0,1); LCD.print(timerRemain);  LCD.setCursor(7,1);  LCD.print("' ");   LCD.setCursor(9,1);    LCD.print(cfg.setTemp, 2);    LCD.setCursor(14,1);    LCD.print(223, BIN);    LCD.setCursor(15,1);    LCD.print("C");
};
void updateLCD(){
    String str;
    LCD.clear();  
    if(!inSetup){
        str = getModeLabel(ncfg.mode);
        str.toCharArray(row1[0],7);
        str = String(inTemp, 1);
        str.toCharArray(row1[1],7);
        str = String(cfg.timer);
        str.toCharArray(row2[0],7);
        str = String(inTemp, 1);
        str.toCharArray(row2[1],7);
    }else{
        switch(posMenuCurent){
            case 101:
                if(lastBlinkState){
                    str = getModeLabel(ncfg.mode);
                    str.toCharArray(row1[0],7);
                }else{
                    str = "";
                    str.toCharArray(row1[0],7);                    
                }
                str = String(inTemp, 1);
                str.toCharArray(row1[1],7);
                str = String(timerRemain);
                str.toCharArray(row2[0],7);
                str = String(inTemp, 1);
                str.toCharArray(row2[1],7);
            break;
            case 103:
                str = getModeLabel(ncfg.mode);
                str.toCharArray(row1[0],7);
                str = String(inTemp, 1);
                str.toCharArray(row1[1],7);
                if(lastBlinkState){
                    str = String(ncfg.timer);
                    str.toCharArray(row2[0],7);
                }else{
                    str = "";
                    str.toCharArray(row2[0],7);                    
                }
                str = String(inTemp, 1);
                str.toCharArray(row2[1],7);              
            break;
            case 102:
                str = getModeLabel(ncfg.mode);
                str.toCharArray(row1[0],7);
                str = String(inTemp, 1);
                str.toCharArray(row1[1],7);
                str = String(ncfg.timer);
                str.toCharArray(row2[0],7);
                if(lastBlinkState){
                    str = String(ncfg.setTemp, 1);
                    str.toCharArray(row2[1],7); 
                }else{
                    str = "";
                    str.toCharArray(row2[1],7);                    
                }
            break;  // mode run
            default: break;
        };
    }
};
String getModeLabel(byte modes){
    if(modes==0)    return "Normal";
    else if(modes==1)    return "Timer";
};
// Model
void createMenuList(){
    maxSubItem[0] = 1;  // 
    maxSubItem[1] = 4;  // Set temp
};
boolean processEncoder(){
    valueEn += encoder->getValue();
    if (valueEn == lastEn) {
        return 0;
        };
    if (!inSetup) {
        lastEn = valueEn;
        return 0;
    };
    int posDelta = valueEn - lastEn;
    lastEn = valueEn;
    if(!inSubMenu){
        posSubMenu = (int)(posSubMenu + posDelta)%MAX_SUBMENU;
        if(!posSubMenu) posSubMenu=1;
    }else if(inSubMenu&&!inSubItem){
        posSubItem = (int)(posSubItem + posDelta)%maxSubItem[posSubMenu];
    }else if(inSubMenu&&inSubItem){
// user insert function for subItem change at here
        switch(posMenuCurent){
            case 101:
                ncfg.mode = (byte)(ncfg.mode + posDelta) % 2; // 2 mode NORMAL, TIMER
            break;  // mode run
            case 102:
                ncfg.setTemp = (float)(ncfg.setTemp + posDelta*0.1);
            break;  // mode run
            case 103:
                ncfg.timer = (float)(ncfg.timer + posDelta*0.1);
            break;  // mode run
        };
    };
    return 1;
};
void processEncoderBtn(){
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch(b){
      case ClickEncoder::Held: handlerHeld(); break;
      case ClickEncoder::Clicked: handlerClicked(); break;
      case ClickEncoder::DoubleClicked: handlerDoubleClicked(); break;
    }; 
  }
};
// Hold button in/out to Setup menu
void handlerHeld(){};
void handlerClicked(){
    updatePosMenu();
    if(!inSetup) {
        inSubMenu = false;
        inSubItem = false;        
        posSubMenu = 0;
        posSubItem = 0;
        return;
    };
    if(inSubMenu){
        if(inSubItem){
            if(posSubItem != 0){
                saveItem();                
            }else{
                inSubMenu = false;
            };
            inSubItem = false;
        }else{
            if(posSubItem != 0){
                inSubItem = true;                
            }else{
                inSubItem = false;
                inSubMenu = false;
            }
        };
    }else{
        inSubMenu = true;
        inSubItem = false;
        posSubItem = 0;
    }
    updatePosMenu();
};
void handlerDoubleClicked(){
    if(inSetup){
        inSetup = false;
        posSubMenu = 0;
    }else{
        inSetup = true;
        posSubMenu = 1;
    }
    inSubMenu = false;
    inSubItem = false;
    updatePosMenu();
};
void updatePosMenu(){
    posMenuCurent = posSubMenu*100 + posSubItem;
    if(!inSubItem){
        ncfg = cfg;
    }
}

