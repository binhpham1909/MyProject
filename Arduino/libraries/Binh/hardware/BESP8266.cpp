#include "BESP8266.h"

//////////////////////////////////////
///   Group function for Hardware  ///
//////////////////////////////////////

// Toggle PIN OUT
// Ex: PIN2=ON/OFF
boolean BESP8266::togglePin(String _pinname, String _value){	// toggle pinout
    uint8_t _pinControl = getPin(_pinname);
    digitalWrite(_pinControl, pinValue(_value));
    return digitalRead(_pinControl);
}
// Convert String PINxx to (int)xx (xx: 0-16)
uint8_t BESP8266::getPin(String _pin){	// Get pin name from string to int
	return _pin.substring(3,_pin.length()).toInt();
}
// Convert String ON/OFF to (boolean)HIGH/LOW
boolean BESP8266::pinValue(String _value){	// Get pin value from string to boolean
	if((_value=="ON")||(_value=="on")){
        return HIGH;}
    else{
        if((_value=="OFF")||(_value=="off")){return LOW;};
    } 
}
// Blink led
void BESP8266::LedBlink(unsigned long _interval){
	if(Timer(&last_blink, _interval)){
		if (LedState == LOW)
			LedState = HIGH;  // Note that this switches the LED *off*
		else if(LedState == HIGH)
			LedState = LOW;   // Note that this switches the LED *on*
		digitalWrite(LEDSTATUS, LedState);         
    }
}
void BESP8266::LedOn(void){
	digitalWrite(LEDSTATUS, HIGH);         
}
void BESP8266::LedOff(void){
	digitalWrite(LEDSTATUS, LOW);         
}
boolean BESP8266::Timer(unsigned long *_last_time, unsigned long _interval){
	unsigned long time_now=millis();
	if((time_now-*_last_time)>=_interval){
		*_last_time=time_now;
		return 1;        
    }else{
		return 0;
	}
}