#include "stdafx.h"

#include "Util/SteppedInstance.h"
#include "FastLog.h"

namespace WBX {

void IStepped::onServiceProviderIStepped(ServiceProvider* oldProvider, ServiceProvider* newProvider)
{
	WBXASSERT((oldProvider == NULL) != (newProvider == NULL));

	steppedConnection.disconnect();

	if (newProvider) {
		RunService* runService = ServiceProvider::create<RunService>(newProvider);
		WBXASSERT(runService);
		FASTLOG1(FLog::ISteppedLifetime, "Subscribed to IStepped", this);

        switch (stepType)
        {
		case StepType_Default:
			steppedConnection = runService->steppedSignal.connect(boost::bind(&IStepped::onStepped, this, _1));
            break;

        case StepType_HighPriority:
			steppedConnection = runService->highPrioritySteppedSignal.connect(boost::bind(&IStepped::onStepped, this, _1));
            break;

        case StepType_Render:
			steppedConnection = runService->renderSteppedSignal.connect(boost::bind(&IStepped::onStepped, this, _1));
            break;

        default:
            WBXASSERT(false);
            break;
        }
	}
}
} // namespace