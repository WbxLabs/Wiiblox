
#pragma once

#include "V8Tree/Instance.h"
#include "Util/RunStateOwner.h"
#include "Util/G3DCore.h"

LOGGROUP(ISteppedLifetime)

namespace WBX {
	
	class IStepped
	{
    public:
        enum StepType
        {
            StepType_Default,
            StepType_HighPriority,
            StepType_Render,
        };

	private:
		StepType stepType;
		wbx::signals::scoped_connection steppedConnection;

	protected:
		// call this inside onServiceProvider
		void onServiceProviderIStepped(ServiceProvider* oldProvider, ServiceProvider* newProvider);

		/*implement*/ virtual void onStepped(const Stepped& event) = 0;

		void stopStepping() {
			steppedConnection.disconnect();
		}

	public:
		IStepped(StepType stepType = StepType_Default): stepType(stepType) {}
		virtual ~IStepped()	{}
	};
}	// namespace WBX