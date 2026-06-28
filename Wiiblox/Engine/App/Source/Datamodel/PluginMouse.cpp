#include "stdafx.h"

#include "V8DataModel/PluginMouse.h"
#include "V8DataModel/PluginManager.h"
#include "V8World/ContactManager.h"
#include "V8DataModel/PartInstance.h"
#include "V8DataModel/Workspace.h"
#include "V8World/World.h"

namespace WBX {

const char *const sPluginMouse = "PluginMouse";

REFLECTION_BEGIN();
static Reflection::EventDesc<PluginMouse, void(shared_ptr<const Instances>)> evt_dragEnterEvent(&PluginMouse::dragEnterEventSignal, "DragEnter", "instances", Security::Plugin);
REFLECTION_END();


PluginMouse::PluginMouse()
{
}

PluginMouse::~PluginMouse()
{
}

void PluginMouse::fireDragEnterEvent(shared_ptr<const wbx::Instances> instances, shared_ptr<InputObject> input)
{
	update(input);
	dragEnterEventSignal(instances);
}

} // namespace

