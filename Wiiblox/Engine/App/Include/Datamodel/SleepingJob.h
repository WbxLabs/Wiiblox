#pragma once

#include "V8DataModel/DataModelJob.h"

namespace WBX {

class SleepingJob : public DataModelJob
{
private:
	wbx::atomic<int> isAwake;
	const double desiredFps;
	wbx::Time lastWakeTime;

public:
	SleepingJob(const char* name, TaskType taskType, bool isPerPlayer,
		shared_ptr<wbx::DataModelArbiter> arbiter, wbx::Time::Interval stepBudget,
		double desiredFps);

	void wake();
	void sleep();

	virtual wbx::Time::Interval sleepTime(const Stats&);

	virtual wbx::TaskScheduler::Job::Error error(const Stats& stats);
};

}
