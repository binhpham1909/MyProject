#ifndef __KCALENDAR__
#define __KCALENDAR__

//Đây là một class dùng kiểu thiết kế hướng đối tượng singleton
struct kJob {
	void (*func)();
	bool (*conditionFunc)();
	unsigned long atTime;
};
class kCalendar {
private:
	long long m_count;
	long long m_lastCheckedJobId;
	unsigned long m_workTime;
	unsigned long m_lastEndTime;
	kJob *m_jobs;
protected:
	void insertJob(kJob &);
  void increaseWorkTime(const int delta);
  void restart();
public:
	static kCalendar* getInstance() {
		static kCalendar *instance = new kCalendar();
		return instance;
	}
	
	virtual void initialize();
	
	virtual void startFirstJob();
	
	virtual void update();
	
	virtual void addJob(void (*func)(), unsigned long time);
};
#endif
