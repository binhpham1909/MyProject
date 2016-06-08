#include <OneWire.h>
#include <PID_v1.h>
// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// http://milesburton.com/Dallas_Temperature_Control_Library
#define SSR_PIN 9
OneWire  ds(4);  // on pin 10 (a 4.7K resistor is necessary)
double Setpoint, Input, Output, LastInput;
//Define the aggressive and conservative Tuning Parameters
double aggKp=4, aggKi=0.2, aggKd=1;
double consKp=1, consKi=0.05, consKd=0.25;
PID myPID(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void setup(void) {
  Serial.begin(115200);
  pinMode(SSR_PIN, OUTPUT);
  Setpoint=60;
  myPID.SetMode(AUTOMATIC);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte type_s;
  byte data[12];
  byte addr[8]; 
  if ( !ds.search(addr)) {
//    Serial.print("No more addresses.");
    ds.reset_search();
    delay(250);
    return;
  }
  
  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!");
      return;
  }
  // the first ROM byte indicates which chip
  switch (addr[0]) {
    case 0x10:
      Serial.print("Chip = DS18S20 ");  // or old DS1820
      type_s = 1;
      break;
    case 0x28:
      Serial.print("Chip = DS18B20 ");
      type_s = 0;
      break;
    case 0x22:
      Serial.print("Chip = DS1822 ");
      type_s = 0;
      break;
    default:
      Serial.print("Device is not a DS18x20 family device.");
      return;
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
  if (type_s) {
    raw = raw << 3; // 9 bit resolution default
    if (data[7] == 0x10) {
      // "count remain" gives full 12 bit resolution
      raw = (raw & 0xFFF0) + 12 - data[6];
    }
  } else {
    byte cfg = (data[4] & 0x60);
    // at lower res, the low bits are undefined, so let's zero them
    if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
    else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
    else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
    //// default is 12 bit resolution, 750 ms conversion time
  }
  Input = (double)raw / 16.0;
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
  for(int i=0;i<(int)Output;i++){
    digitalWrite(SSR_PIN, HIGH);
    Serial.print("+");
    delay(10);
  }
  for(int i=(int)Output;i<=255;i++){
    digitalWrite(SSR_PIN, LOW);
//    Serial.print("-");
    delay(10);
  }
  digitalWrite(SSR_PIN, LOW);
  Serial.print("Temperature = ");
  Serial.print(Input);
  Serial.print(" C \t");
  Serial.print("PID Output = ");
  Serial.print(Output);
  Serial.print(",\t SSR :");
  if(digitalRead(SSR_PIN)){Serial.println("ON");}else{Serial.println("OFF");}
}
