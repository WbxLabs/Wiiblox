#include "stdafx.h"

#include "V8DataModel/SleepingJob.h"

namespace WBX {

SleepingJob::SleepingJob(const char* name, TaskType taskType, bool isPerPlayer,
		shared_ptr<wbx::DataModelArbiter> arbiter, wbx::Time::Interval stepBudget,
		double desiredFps)
	: wbx::DataModelJob(name, taskType, isPerPlayer, arbiter, stepBudget)
	, desiredFps(desiredFps) {
	isAwake = false;
}

void SleepingJob::wake() { 
	if (!isAwake.compare_and_swap(1,0))
	{
		lastWakeTime = wbx::Time::now<wbx::Time::Fast>();
		wbx::TaskScheduler::singleton().reschedule(shared_from_this());
	}
}

void SleepingJob::sleep() {
	isAwake = false;
}

Time::Interval SleepingJob::sleepTime(const Stats&) {
	return isAwake ? wbx::Time::Interval(0) : wbx::Time::Interval::max();
}

TaskScheduler::Job::Error SleepingJob::error(const Stats& stats) {
	if (!isAwake)
		return Job::Error();
	
	Stats fakedStats = stats;
	wbx::Time::Interval timeSinceAwoke = wbx::Time::now<wbx::Time::Fast>() - lastWakeTime;
	if (timeSinceAwoke < fakedStats.timespanSinceLastStep) {
		fakedStats.timespanSinceLastStep = timeSinceAwoke;
	}

	return computeStandardError(fakedStats, desiredFps);
}

}