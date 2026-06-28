#include "stdafx.h"

#include "Humanoid/RunningNoPhysics.h"
#include "Humanoid/Humanoid.h"

DYNAMIC_FASTFLAG(ClampRunSignalMinSpeed)

namespace WBX {
namespace HUMAN {

const char* const sRunningNoPhysics = "RunningNoPhysics";


RunningNoPhysics::RunningNoPhysics(Humanoid* humanoid, StateType priorState)
	:Named<MovingNoPhysicsBase, sRunningNoPhysics>(humanoid, priorState)
{
	if (DFFlag::ClampRunSignalMinSpeed)
	{
		fireMovementSignal(humanoid->runningSignal, getRelativeMovementVelocity().xz().length() );
	
	}
	else
	{
		humanoid->runningSignal(getRelativeMovementVelocity().xz().length());
	}
}



} // namespace HUMAN
} // namespace WBX