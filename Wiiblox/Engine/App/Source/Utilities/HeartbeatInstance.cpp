#include "stdafx.h"

#include "Util/HeartbeatInstance.h"
#include "Util/RunStateOwner.h"

namespace WBX {

void HeartbeatInstance::onServiceProviderHeartbeatInstance(ServiceProvider* oldProvider, ServiceProvider* newProvider)
{
	WBXASSERT((oldProvider == NULL) != (newProvider == NULL));

	heartbeatConnection.disconnect();

	if (newProvider) {
		RunService* runService = ServiceProvider::create<RunService>(newProvider);
		WBXASSERT(runService);
		heartbeatConnection = runService->heartbeatSignal.connect(boost::bind(&HeartbeatInstance::onHeartbeat, this, _1));
	}
}

} // namespace