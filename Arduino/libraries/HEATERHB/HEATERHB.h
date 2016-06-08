#ifndef TEMPCONTROL_h
#define TEMPCONTROL_h
#include "Arduino.h"

#define BAT(X) digitalWrite(X, LOW)  
#define TAT(X) digitalWrite(X, HIGH) 

enum deviceMode{
	heat = 1,
	cool,
	mix
};
class TEMPCONTROL	// class chua cac ham xu ly cua thu vien
{
	public:
		void setup(uint8_t _hotPin, uint8_t _coolPin, uint8_t _valPin, deviceMode _mode);
		void setTemp(float _temp);
		void updateTemp(float _tempIn, float _tempOut);
		void run(void);
		void stop(void);
	private:
		deviceMode mode;
		float tempSet, tempIn, tempOut;
		uint8_t PIN_HOT, PIN_COOL, PIN_VAL_COOL;
		boolean hotOn, coolOn, valOn;
};
#endif