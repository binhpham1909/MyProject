#include "TEMPCONTROL.h"

void TEMPCONTROL::setup(uint8_t _hotPin, uint8_t _coolPin, uint8_t _valPin, deviceMode _mode){
	PIN_HOT = _hotPin;
	PIN_COOL = _coolPin;
	PIN_VAL_COOL = _valPin;
	pinMode(PIN_HOT, OUTPUT);  digitalWrite(PIN_HOT, HIGH);
    pinMode(PIN_COOL, OUTPUT); digitalWrite(PIN_COOL, HIGH); 
    pinMode(PIN_VAL_COOL, OUTPUT); digitalWrite(PIN_VAL_COOL, HIGH); 
	mode = _mode;
};
void TEMPCONTROL::setTemp(float _temp){
	tempSet = _temp;
};
void TEMPCONTROL::updateTemp(float _tempIn, float _tempOut){
	tempIn = _tempIn;
	tempOut = _tempOut;
};
void TEMPCONTROL::run(void){
    if(tempSet < tempOut){  // che do lam mat
        hotOn = false;    // tat phan lam nong
		coolOn = true;	// bat phan lam mat
        if(tempIn > tempSet){   // neu chua mat
            valOn = true;    // mo van lam mat
        }else{  // neu mat roi
            valOn = false;    // tat van lam mat
        }
    }else{  // che do lam nong
		if(coolOn && ((tempSet - tempOut) > 2)){
			coolOn = false;
		}
        if(tempIn > tempSet){   // neu qua nong
			hotOn = false;    // tat nhiet
		}else{  // neu chua du nong
			hotOn = true;    // cap nhiet
		}
	}
	digitalWrite(PIN_HOT, !hotOn);
	digitalWrite(PIN_COOL, !coolOn); 
	digitalWrite(PIN_VAL_COOL, !valOn); 
};
void TEMPCONTROL::stop(void){
	digitalWrite(PIN_HOT, HIGH);
	digitalWrite(PIN_COOL, HIGH); 
	digitalWrite(PIN_VAL_COOL, HIGH); 	
}