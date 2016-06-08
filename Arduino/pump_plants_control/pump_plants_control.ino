#include <EEPROM.h>
//#include <LCD5110_Basic.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <avr/pgmspace.h>
#include <ClickEncoder.h>
#include <TimerOne.h>
#include <Time.h>
#include <DS1307RTC.h>
// PIN connector
// LCD function define - pin 10 11 12 14(A0) 15(A1)
//      SCK  - Pin 8  17
//      MOSI - Pin 9  16
//      DC   - Pin 10 15
//      RST  - Pin 11 11
//      CS   - Pin 12 14
#define LCD_CLK 10
#define LCD_DIN  11
#define LCD_DC 12
#define LCD_RST 14
#define LCD_CE 13

// Relay pin
#define PUMP_PIN 5
#define LIGHT_PIN 6

// Sensor pin
#define MOISTURE_PIN A3

// Button pin
#define AUTO_BTN 15 //A1

// Rotaty Encoder PIN
#define A_PIN 9
#define B_PIN 8
#define BTN_PIN 7
// RTC PIN
#define RTC_SCL 3
#define RTC_SDA 4
//#define RTC_SCK 2

// Text display menu
const char lb_menu0[] PROGMEM="Pum Control";
const char lb_menu1[] PROGMEM="Mode run";
const char lb_menu2[] PROGMEM="Do am";
const char lb_menu3[] PROGMEM="Timer";
const char lb_menu4[] PROGMEM="Time Enable";
const char lb_menu5[] PROGMEM="Time Disable";
const char lb_menu6[] PROGMEM="Clock time";
const char lb_menu7[] PROGMEM="Clock date";
const char lb_menu8[] PROGMEM="Calibration";
const char* const lb_menu[] PROGMEM ={lb_menu0, lb_menu1, lb_menu2, lb_menu3, lb_menu4, lb_menu5, lb_menu6, lb_menu7, lb_menu8};
unsigned long lastView;
// define
#define MAX_SUBMENU  9
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

//LCD5110 LCD(LCD_SCK,LCD_MOSI,LCD_DC,LCD_RST,LCD_CS);
Adafruit_PCD8544 LCD = Adafruit_PCD8544(LCD_CLK, LCD_DIN, LCD_DC, LCD_CE, LCD_RST);

//extern uint8_t HBIlogo[];
//extern uint8_t SmallFont[];
//extern uint8_t Arial10In[];
//extern uint8_t Arial10[];
char row[6][14];
// EEPROM
#define _CONFIGS_ 0    
struct configs{
  byte Mode;
  int adc0;
  int adc100;
  float moisSet;
  float moisOffset;
  int timerOn;
  int timerOff;
  byte HHEnable;
  byte MMEnable;
  byte HHDisable;
  byte MMDisable;
} cfg, ncfg;
// Encoder
ClickEncoder *encoder;
int16_t lastEn, valueEn;

// Create a DS1302 object.
//DS1302 rtc(RTC_RST,RTC_IO,RTC_SCK);
tmElements_t t_now;
tmElements_t nTime;

// pump
boolean pumpNow = false;
boolean lastTimerState = false;
long tEnable, tDisable, tNow, tLastTimer;
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
    createMenuList();
}

void loop(void) {
    tmElements_t tm;
    if (RTC.read(tm)) t_now = tm;
    tNow = t_now.Hour*3600 + t_now.Minute*60 + t_now.Second;
    processEncoder();
    processEncoderBtn();
    updateLCD();
    displayLCD();
    controlPump();
    LCD.display();
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
        Serial.print(br);   Serial.print(F("mode"));   Serial.print(br);   Serial.print(se);     Serial.print(br);   Serial.print(cfg.Mode);   Serial.print(br); Serial.print(ne);
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
        ncfg.Mode = val.toInt();
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
    LCD.begin();
    LCD.setContrast(70);
    LCD.display(); // show splashscreen
    delay(2000);
    LCD.clearDisplay();   // clears the screen and buffer
    // text display tests
    LCD.setTextSize(2);
    LCD.setTextColor(BLACK);
    LCD.setCursor(30,10);
    LCD.println(F("HBI"));
    LCD.setTextSize(1);
    LCD.setTextColor(BLACK);
    LCD.setCursor(0,30);
    LCD.println(F("HBInvent.vn"));
    LCD.display();
    delay(1000);
};
void initECRotaty(){
    encoder = new ClickEncoder(A_PIN,B_PIN,BTN_PIN,4);
    encoder->setAccelerationEnabled(true);
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr);
    lastEn = -1;    
};
void initGPIO(){
    pinMode(LIGHT_PIN, OUTPUT);  digitalWrite(LIGHT_PIN, HIGH);
    pinMode(PUMP_PIN, OUTPUT); digitalWrite(PUMP_PIN, HIGH);    
};
/// Menu with encoder rotaty - MVC
int readADC(){
    return analogRead(MOISTURE_PIN);
};
float readMoisture(){
    return (100*(analogRead(MOISTURE_PIN)-cfg.adc0)/(cfg.adc100-cfg.adc0));
};
// Control
void controlPump(){
    float nowMois = readMoisture();
    tEnable = cfg.HHEnable*3600 + cfg.MMEnable*60;
    tDisable = cfg.HHDisable*3600 + cfg.MMDisable*60;
    if((cfg.Mode == 0)   ){// external button
        pumpNowMan = false;
        pumpNowTimer = false;
        pumpNowAuto = false;
    }else if(cfg.Mode == 1){
        pumpNowMan = false;
        pumpNowTimer = false;        
        if(nowMois <= (cfg.moisSet - cfg.moisOffset)){
            pumpNowAuto = true;
        };
        if(nowMois > (cfg.moisSet + cfg.moisOffset)){
            pumpNowAuto = false;
        }
    }else if((cfg.Mode == 2)){
        pumpNowMan = false;
        pumpNowAuto = false;
        if(lastTimerState){
            if(tNow - tLastTimer > cfg.timerOn*60){
                pumpNowTimer = false;
                tLastTimer = tNow;
                lastTimerState = false;
            }else{
                pumpNowTimer = true;
            }
        }else{
            if(tNow - tLastTimer > cfg.timerOff*60){
                pumpNowTimer = true;
                tLastTimer = tNow;
                lastTimerState = true;
            }else{
                pumpNowTimer = false;
            }
        };
    }
    if(tEnable == tDisable){
        pumpAllow = true;
    }else if(tEnable > tDisable){
        if((tNow >= tDisable)&&(tNow <= tEnable)){
            pumpAllow = false;
        }else{
            pumpAllow = true;
        }
    }else{
        if(((tNow >= tDisable)&&(tNow <= 86399))||((tNow >= 0)&&(tNow <= tEnable))){
            pumpAllow = false;
        }else{
            pumpAllow = true;
        }
    };
    pumpNow = (pumpNowMan||pumpNowAuto||pumpNowTimer)&&pumpAllow;
    digitalWrite(PUMP_PIN, !pumpNow);
};

void timerIsr() {
  encoder->service();
};
void saveItem(){
    switch(posSubMenu){
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 8:
                {
                    EEPROM.put( _CONFIGS_, ncfg );
                    EEPROM.get( _CONFIGS_, cfg );              
                }
                break; 
            case 6:
                nTime.Day = t_now.Day;
                nTime.Month = t_now.Month;
                nTime.Year = t_now.Year;
                RTC.write(nTime);
                nTime = t_now;
            break;  // mode run
            case 7:  
                nTime.Hour = t_now.Hour;
                nTime.Minute = t_now.Minute;
                nTime.Second = t_now.Second;            
                RTC.write(nTime);
                nTime = t_now;
            break;
            default: break;
    };
};
// View
void displayLCD(){
    LCD.clearDisplay();
    LCD.setTextSize(1);
    if(posSubItem==0){  LCD.setTextColor(WHITE, BLACK); }else{  LCD.setTextColor(BLACK);}; LCD.setCursor(0,0); LCD.print(row[0]);
    if(posSubItem==1){  LCD.setTextColor(WHITE, BLACK); }else{  LCD.setTextColor(BLACK);}; LCD.setCursor(0,16); LCD.print(row[1]);
    if(posSubItem==2){  LCD.setTextColor(WHITE, BLACK); }else{  LCD.setTextColor(BLACK);}; LCD.setCursor(0,24); LCD.print(row[2]);
    if(posSubItem==3){  LCD.setTextColor(WHITE, BLACK); }else{  LCD.setTextColor(BLACK);}; LCD.setCursor(0,32); LCD.print(row[3]);
    if(posSubItem==4){  LCD.setTextColor(WHITE, BLACK); }else{  LCD.setTextColor(BLACK);}; LCD.setCursor(0,40); LCD.print(row[4]);
    if(posSubItem==5){  LCD.setTextColor(WHITE, BLACK); }else{  LCD.setTextColor(BLACK);}; LCD.setCursor(0,8); LCD.print(row[5]);
};
void updateLCD(){
    String str;   
    if(!inSetup){
//        LCD.setFont(SmallFont); // font 6x8
        if(cfg.Mode == 2){
            if(lastTimerState){
                str = "Wait :" + String(tLastTimer + cfg.timerOn*60 - tNow) + " s";
            }else{
                str = "Wait :" + String(tLastTimer + cfg.timerOff*60 - tNow) + " s";
            }
        }else{
            str = "M%: "+String(readMoisture());           
        }
        str.toCharArray(row[3],13); 
        str = getModeLabel(cfg.Mode);
        str.toCharArray(row[0],13);
        str = String(t_now.Day) + "-" + String(t_now.Month) + "-" + String(t_now.Year);
        str.toCharArray(row[1],13);
        str = String(t_now.Hour) + ":" + String(t_now.Minute) + ":" + String(t_now.Second);
        str.toCharArray(row[2],13);
        if(pumpNow){
            strcpy(row[4], "PUMP: ON");
        }
        else{
            strcpy(row[4], "PUMP: OFF");
        };
        if(pumpAllow){
            strcpy(row[5], "Time Enable");
        }else{
            strcpy(row[5], "Time Disable");
        };
    }else{
        switch(posSubMenu){
            case 1:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                strcpy(row[2], "");
                str = getModeLabel(ncfg.Mode);
                str.toCharArray(row[1],13);
                strcpy(row[3], "");
                strcpy(row[4], "");
                strcpy(row[5], "");
            break;
            case 2:  
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "Do am: "+ String(ncfg.moisSet);
                str.toCharArray(row[1],13);
                str = "Offset: "+ String(ncfg.moisOffset);
                str.toCharArray(row[2],13);
                strcpy(row[3], "");
                strcpy(row[4], "");
                strcpy(row[5], "");            
            break;
            case 3:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "ON : "+ String(ncfg.timerOn);
                str.toCharArray(row[1],13);
                str = "OFF: "+ String(ncfg.timerOff);
                str.toCharArray(row[2],13);
                strcpy(row[3], "");
                strcpy(row[4], "");
                strcpy(row[5], "");             
            break;
            case 4:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "HH: "+ String(ncfg.HHEnable);
                str.toCharArray(row[1],13);
                str = "MM: "+ String(ncfg.MMEnable);
                str.toCharArray(row[2],13);
                strcpy(row[3], "");
                strcpy(row[4], ""); 
                strcpy(row[5], "");            
            break;  // mode run
            case 5:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "HH: "+ String(ncfg.HHDisable);
                str.toCharArray(row[1],13);
                str = "MM: "+ String(ncfg.MMDisable);
                str.toCharArray(row[2],13);
                strcpy(row[3], "");
                strcpy(row[4], ""); 
                strcpy(row[5], "");            
            break;  // mode run
            case 6:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "HH: "+ String(nTime.Hour);
                str.toCharArray(row[1],13);
                str = "MM: "+ String(nTime.Minute);
                str.toCharArray(row[2],13);
                str = "SS: "+ String(nTime.Second);
                str.toCharArray(row[3],13);
                strcpy(row[4], ""); 
                strcpy(row[5], "");            
            break;
            case 7:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "dd  : "+ String(nTime.Day);
                str.toCharArray(row[1],13);
                str = "mm  : "+ String(nTime.Month);
                str.toCharArray(row[2],13);
                str = "yyyy: "+ String(nTime.Year);
                str.toCharArray(row[3],13);
                strcpy(row[4], ""); 
                strcpy(row[5], "");            
            break;
            case 8:
                strcpy_P(row[0], (char*)pgm_read_word(&(lb_menu[posSubMenu])));
                str = "0%  : "+ String(ncfg.adc0);
                str.toCharArray(row[1],13);
                str = "100%: "+ String(ncfg.adc100);
                str.toCharArray(row[2],13);
                if(((int)(millis()/500))%2 == 1){
                    str = "Sensor : "+ String(analogRead(MOISTURE_PIN));
                    str.toCharArray(row[4],13);                    
                }
                strcpy(row[3], ""); 
                strcpy(row[5], "");            
            break;
            default: break;
        };
    }
};
String getModeLabel(byte modes){
    if(modes==0)    return "Manual";
    else if(modes==1)    return "Auto";
    else if(modes==2)    return "Timer";
    else return "None";
};
// Model
void createMenuList(){
    maxSubItem[0] = 1;  // 
    maxSubItem[1] = 2;  // Set mode run
    maxSubItem[2] = 3;  // Set moisture: moisture, offset
    maxSubItem[3] = 3;  // Set timer: time on, time off
    maxSubItem[4] = 3;  // Set time enable: hh, mm
    maxSubItem[5] = 3;  // Set time disable: hh, mm
    maxSubItem[6] = 4;  // Set RTC clock: hh, mm, ss
    maxSubItem[7] = 4;  // Set RTC clock: dd, mm, yy
    maxSubItem[8] = 3;  // Calibration: adc0, adc100
//    maxSubItem[7] = 11;  // Program: adc0, adc100
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
                ncfg.Mode = (byte)(ncfg.Mode + posDelta) % 3; // 3mode MANUAL, AUTO, TIMER
            break;  // mode run
            case 201:
                ncfg.moisSet = (float)(ncfg.moisSet + posDelta*0.1);
            break;  // mode run
            case 202:
                ncfg.moisOffset = (float)(ncfg.moisOffset + posDelta*0.1);
            break;
            case 301:
                ncfg.timerOn = (int)(ncfg.timerOn + posDelta);
            break;  // mode run
            case 302:
                ncfg.timerOff = (int)(ncfg.timerOff + posDelta);
            break;
            case 401:
                ncfg.HHEnable = (byte)(ncfg.HHEnable + posDelta) % 24;
            break;  // mode run
            case 402:
                ncfg.MMEnable = (byte)(ncfg.MMEnable + posDelta) % 60;
            break;
            case 501:
                ncfg.HHDisable = (byte)(ncfg.HHDisable + posDelta) % 24;
            break;  // mode run
            case 502:
                ncfg.MMDisable = (byte)(ncfg.MMDisable + posDelta) % 60;
            break;
            case 601:
                nTime.Hour = (byte)(nTime.Hour + posDelta) % 24;
            break;  // mode run
            case 602:  
                nTime.Minute = (byte)(nTime.Minute + posDelta) % 60;
            break;
            case 603:
                nTime.Second = (byte)(nTime.Second + posDelta) % 60;
            break;
            case 701:
                nTime.Day = (byte)(nTime.Day + posDelta) % 32;
            break;  // mode run
            case 702:  
                nTime.Month = (byte)(nTime.Month + posDelta) % 13;
            break;
            case 703:
                nTime.Year = (byte)(nTime.Year + posDelta) % 100;
            break;
            case 801:
            //    ncfg.adc0 = analogRead(MOISTURE_PIN);
                ncfg.adc0 = (int)(ncfg.adc0 + posDelta) % 1024;
            break;  // mode run
            case 802:
            //    ncfg.adc100 = analogRead(MOISTURE_PIN);
                ncfg.adc100 = (int)(ncfg.adc100 + posDelta) % 1024;
            break;
        };
    };
    return 1;
};
void processEncoderBtn(){
  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch(b){
      case ClickEncoder::Pressed: handlerPressed(); break;
      case ClickEncoder::Held: handlerHeld(); break;
      case ClickEncoder::Released: handlerReleased(); break;
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
void handlerPressed(){};
void handlerReleased(){};
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
        nTime = t_now;
    }
}

