#ifndef __KINDUSTRYCALENDAR__
#define __KINDUSTRYCALENDAR__
#include "kCalendar.h"
class kIndustryCalendar: public kCalendar {
private:
	unsigned long m_lastTimePointBeAdded;
	unsigned long m_maxProcessCount;
	unsigned long m_processCount;			//số lượng process đã thực hiện
	byte m_status;							//trạng thái
public:
	static kIndustryCalendar* getInstance() {
		static kIndustryCalendar *instance = new kIndustryCalendar();
		return instance;
	}
	
	virtual void initialize();
	
	virtual void addJob(void (*func)(), unsigned long time);
	
	virtual void addJob(void (*func)(), bool (*conditionFunc)(), unsigned long time = 0);
	
	virtual void startFirstJob();
	
	virtual void update();
	
	virtual void setProcessCount(const unsigned long maxProcessCount);
	
	virtual void resetProcessCount();
	
	bool isPause() const ;
	
	bool isStop() const;
	
	bool isRunning() const;
	
	virtual void pause();
	
	virtual void startAgain();
	
	virtual void start();
	
	virtual void stop();
};

#endif
