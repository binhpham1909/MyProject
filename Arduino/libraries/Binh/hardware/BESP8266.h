#ifndef BESP8266_h
#define BESP8266_h

#include <ESP8266WiFi.h>

class BESP8266{
    public:
        boolean togglePin(String _pinname, String _value);
        uint8_t getPin(String _pin);
        boolean pinValue(String _value);
        void LedBlink(unsigned long _interval);
        void LedOn(void);
        void LedOff(void);
        boolean Timer(unsigned long *_last_time, unsigned long _interval);        
    private:
		unsigned long last_blink=0;
		boolean LedState=LOW;
		unsigned char LEDSTATUS;                
};
#endif