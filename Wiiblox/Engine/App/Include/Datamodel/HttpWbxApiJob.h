#include "V8Tree/Instance.h"
#include "v8datamodel/DataModelJob.h"
#include "v8datamodel/DataModel.h"
#include "v8datamodel/HttpWbxApiService.h"

DYNAMIC_FASTINTVARIABLE(HttpWbxApiJobFrequencyInSeconds, 1);

namespace WBX {


class HttpWbxApiJob : public DataModelJob
{
	shared_ptr<HttpWbxApiService> apiService;

	int lastJobFrequency;
	double desiredHz;

public:
	HttpWbxApiJob(HttpWbxApiService* owner)
		: DataModelJob("HttpWbxApiJob", DataModelJob::Write, false, shared_from(DataModel::get(owner)), Time::Interval(0.01))
		, apiService(shared_from(owner))
	{
		updateHz();
	}

	void updateHz()
	{
		desiredHz = 1.0f / DFInt::HttpWbxApiJobFrequencyInSeconds;
		lastJobFrequency = DFInt::HttpWbxApiJobFrequencyInSeconds;
	}

	/*override*/ Time::Interval sleepTime(const Stats& stats)
	{
		return computeStandardSleepTime(stats, desiredHz);
	}
	/*override*/ Job::Error error(const Stats& stats)
	{
		return computeStandardError(stats, desiredHz);
	}
	/*override*/ TaskScheduler::StepResult stepDataModelJob(const Stats& stats)
	{
		if (DFInt::HttpWbxApiJobFrequencyInSeconds != lastJobFrequency)
			updateHz();

		apiService->addThrottlingBudgets(DFInt::HttpWbxApiJobFrequencyInSeconds / 60.0f);
		apiService->executeThrottledRequests();
		apiService->executeRetryRequests();

		return TaskScheduler::Stepped;
	}
};

} // namespace WBX