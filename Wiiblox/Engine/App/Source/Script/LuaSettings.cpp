#include "stdafx.h"

#include "Script/LuaSettings.h"
#include "LuaConf.h"

const char *const wbx::sLuaSettings = "LuaSettings";

REFLECTION_BEGIN();
static wbx::Reflection::BoundProp<int> prop_1("GcPause", "Garbage Collection", &wbx::LuaSettings::gcPause);
static wbx::Reflection::BoundProp<int> prop_2("GcStepMul", "Garbage Collection", &wbx::LuaSettings::gcStepMul);
static wbx::Reflection::BoundProp<double> prop_3("DefaultWaitTime", "Settings", &wbx::LuaSettings::defaultWaitTime);
static wbx::Reflection::BoundProp<int> prop_4("GcLimit", "Garbage Collection", &wbx::LuaSettings::gcLimit);
static wbx::Reflection::BoundProp<int> prop_5("GcFrequency", "Garbage Collection", &wbx::LuaSettings::gcFrequency);
static wbx::Reflection::BoundProp<bool> prop_areScriptStartsReported("AreScriptStartsReported", "Diagnostics", &wbx::LuaSettings::areScriptStartsReported);
static wbx::Reflection::BoundProp<float> prop_AreWaitingThreadsBudgeted("WaitingThreadsBudget", "Settings", &wbx::LuaSettings::waitingThreadsBudget);
REFLECTION_END();

wbx::LuaSettings::LuaSettings(void)
	:gcPause(LUAI_GCPAUSE)
	,gcStepMul(LUAI_GCMUL)
	,defaultWaitTime(0.03)
    ,smallestWaitTime(0.016667)
	,gcLimit(2)
	,gcFrequency(0)
	,areScriptStartsReported(false)
	,waitingThreadsBudget(0.1)
{
	setName("Lua");
}
