#include "BWorks.h"

bool trueFunc() {
	return true;
}

void BWorks::run() {
	m_workTime += Timer::getInstance()->delta();
	for (uint16_t i 0; i < numJob; i++) {
		nowTimes = millis();
		ListJob job = listJob[i];
		if (((nowTimes - job.lastRun)>=job.interval)&&job.conditionFunc()) {
			job.lastRun = nowTimes;
			job.func();
		}
	}
}

void BWorks::addJob(void (*func)(), uint32_t interval) {
	ListJob job = {func, trueFunc, interval, (uint32_t) 0};
	insertJob(job);
}
void BWorks::addJob(void (*func)(), bool (*conditionFunc)(), uint32_t interval) {
	ListJob job = {func, conditionFunc, interval, (uint32_t) 0};
	insertJob(job);
}
void BWorks::insertJob(ListJob &job) {
	numJob++;
	ListJob *jobs = new ListJob[numJob];
	if (numJob > 1)
		for (uint16_t i = 0; i < numJob - 1; i++)
			jobs[i] = listJob[i];
	
	jobs[numJob - 1] = job;
	delete listJob;
	listJob = jobs;
}