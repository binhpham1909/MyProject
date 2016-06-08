/* DHT library

MIT license
written by Adafruit Industries
*/
#ifndef DHT_H
#define DHT_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


// Uncomment to enable printing out nice debug messages.
//#define DHT_DEBUG

#ifdef DEBUG_ESP_HBI
#ifdef DEBUG_ESP_PORT
#define DEBUG_HBI(...) DEBUG_ESP_PORT.print( __VA_ARGS__ )
#define DEBUGln_HBI(...) DEBUG_ESP_PORT.println( __VA_ARGS__ )


#endif
#endif

#ifndef DEBUG_HBI
#define DEBUG_HBI(...)
#define DEBUGln_HBI(...)
#endif

// Define types of sensors.
#define DHT11 11
#define DHT22 22
#define DHT21 21
#define AM2301 21


class DHT {
  public:
   void init(uint8_t pin, uint8_t type, uint8_t count, boolean DEBUG);
   float readTemperature(bool S=false, bool force=false);
   float convertCtoF(float);
   float convertFtoC(float);
   float computeHeatIndex(float temperature, float percentHumidity, bool isFahrenheit=true);
   float readHumidity(bool force=false);
   boolean read(bool force=false);

 private:
  uint8_t data[5];
  uint8_t DHT_PIN, DHT_TYPE;
  #ifdef __AVR
    // Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask
    // for the digital pin connected to the DHT.  Other platforms will use digitalRead.
    uint8_t _bit, _port;
  #endif
  uint32_t DHT_LASTREAD_TIME, DHT_MAX_CYCLES;
  bool DHT_LAST_RESULT,DEBUG;
  uint32_t expectPulse(bool level);

};

class InterruptLock {
  public:
   InterruptLock() {
    noInterrupts();
   }
   ~InterruptLock() {
    interrupts();
   }

};

#endif
