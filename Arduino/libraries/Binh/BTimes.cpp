#include "BTimes.h"
BTimes::BTimes(uint32_t _period){
	period = _period;
}

void BTimes::resetTimes(void){
	nowTimes = millis();
	lastTimes = nowTimes;
}

void BTimes::checkTimes(void){
	nowTimes = millis();
	if((nowTimes - lastTimes)>=period){
		resetTimes();
		return true;
	}else{
		return false;
	}
}