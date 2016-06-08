#include <Arduino.h>
struct BTimes_s{
	uint32_t lastTimes;
	uint32_t nowTimes;
	uint32_t period;
}

class BTimes{
	public:
		BTimes(uint32_t _period);
		void resetTimes(void);
		boolean checkTimes(void);
	private:
		uint32_t lastTimes;
		uint32_t nowTimes;
		uint32_t period;
};