#include<Arduino.h>
#include "kIndustryCalendar.h"
#include "Timer.h"

void kIndustryCalendar::initialize() {
  m_lastTimePointBeAdded = 0;
  m_maxProcessCount = 0;		//đặt số lượng process sẽ phải thực hiện. 0 là sẽ thực hiện mãi.
  m_processCount = 0;
  m_status = 2;
  kCalendar::initialize();
}

void kIndustryCalendar::addJob(void (*func)(), unsigned long time) {
  m_lastTimePointBeAdded += time;
  kCalendar::addJob(func, m_lastTimePointBeAdded);
}

void kIndustryCalendar::addJob(void (*func)(), bool (*conditionFunc)(), unsigned long time) {
  m_lastTimePointBeAdded += time;
  kJob job = {func, conditionFunc, m_lastTimePointBeAdded};
  insertJob(job);
}

void kIndustryCalendar::startFirstJob() {
	kCalendar::startFirstJob();
	m_processCount++;
	m_status = 0;
}

void kIndustryCalendar::update() {
  static bool setPause = false;
	if ((m_maxProcessCount == 0 || m_processCount <= m_maxProcessCount) && this->isRunning()) {
		kCalendar::update();
    setPause = true;
	} else {
    kCalendar::increaseWorkTime(Timer::getInstance()->delta());
  	if (setPause) {
      resetProcessCount();
      setPause = false;
  		m_status = 2;
  	}
	}
}

void kIndustryCalendar::setProcessCount(const unsigned long maxProcessCount) {
	m_maxProcessCount = maxProcessCount;
}

void kIndustryCalendar::resetProcessCount() {
	m_processCount = 0;
}

bool kIndustryCalendar::isPause() const {
  return m_status == 1;
}

bool kIndustryCalendar::isStop() const {
  return m_status == 2;
}

bool kIndustryCalendar::isRunning() const {
  return m_status == 0;
}

void kIndustryCalendar::pause() {
  if (Serial)
    Serial.println("Tam dung");
  m_status = 1;       //tạm dừng
}

void kIndustryCalendar::startAgain() {
  start();  
  startFirstJob();
}

void kIndustryCalendar::start() {
  if (Serial)
    Serial.println("Tiep tuc chay");
  m_status = 0;   //tiếp tục chạy
}

void kIndustryCalendar::stop() {
  if (Serial)
    Serial.println("Huy toan bo tien trinh va dung lai");
  m_status = 2; //stop
  m_processCount = 0;
  kCalendar::restart();
}
