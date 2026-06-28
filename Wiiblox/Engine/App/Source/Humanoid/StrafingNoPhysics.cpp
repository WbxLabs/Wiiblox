#include "stdafx.h"

#include "Humanoid/StrafingNoPhysics.h"
#include "Humanoid/Humanoid.h"

namespace WBX {
namespace HUMAN {

const char* const sStrafingNoPhysics = "StrafingNoPhysics";


StrafingNoPhysics::StrafingNoPhysics(Humanoid* humanoid, StateType priorState)
	:Named<MovingNoPhysicsBase, sStrafingNoPhysics>(humanoid, priorState)
{
}


} // namespace HUMAN
} // namespace WBX