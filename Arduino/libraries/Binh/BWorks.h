#include <Arduino.h>
struct ListJob {
	void (*func)();
	bool (*conditionFunc)();
	uint32_t interval;
	uint32_t lastRun;
};

class BWorks{
	private:
		ListJob *listJob;
		uint16_t numJob;
		void insertJob(ListJob &);
	public:
		void addJob(void (*func)(), uint32_t interval);
		void addJob(void (*func)(), bool (*conditionFunc)(),uint32_t interval);
		void run();
};