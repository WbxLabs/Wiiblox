#include "stdafx.h"

#include "V8World/IPipelined.h"
#include "V8Kernel/Kernel.h"

namespace WBX {


Kernel* IPipelined::getKernel() const				
{
	WBXASSERT(this->inKernel());
	IStage* answer = getStage(IStage::KERNEL_STAGE);
	return WBX_static_cast<Kernel*>(answer);
}

void IPipelined::putInKernel(Kernel* kernel)
{
	putInStage(kernel);
}

void IPipelined::removeFromKernel()
{
	WBXASSERT(currentStage);
	WBXASSERT(currentStage->getStageType() == IStage::KERNEL_STAGE);
	removeFromStage(currentStage);
}



IStage* IPipelined::getStage(IStage::StageType stageType) const
{
	WBXASSERT(currentStage);
	IStage* tryStage = currentStage;
	do {
		if (tryStage->getStageType() == stageType) {
			return tryStage;
		}
		tryStage = (tryStage->getStageType() > stageType)
						?	tryStage->getUpstream()
						:	tryStage->getDownstream();
	}
	while (1);
}

void IPipelined::putInPipeline(IStage* stage)
{
	WBXASSERT(stage);
	WBXASSERT(!currentStage);
	currentStage = stage;
}

void IPipelined::removeFromPipeline(IStage* stage)
{
	WBXASSERT(stage);
	WBXASSERT(currentStage);
	WBXASSERT(currentStage == stage);
	currentStage = NULL;
}


void IPipelined::putInStage(IStage* stage)
{
	WBXASSERT(stage);
	WBXASSERT(currentStage);
	WBXASSERT(stage->getUpstream() == currentStage);
	WBXASSERT(currentStage->getDownstream() == stage);
	currentStage = stage;
}

void IPipelined::removeFromStage(IStage* stage)
{
	WBXASSERT(currentStage);
	WBXASSERT(stage);
	WBXASSERT(stage == currentStage);
	WBXASSERT(stage->getUpstream());
	currentStage = currentStage->getUpstream();
}

} // namespace