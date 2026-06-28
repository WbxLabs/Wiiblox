#include "stdafx.h"

#include "V8DataModel/BasicPartInstance.h"
#include "V8DataModel/ExtrudedPartInstance.h"
#include "V8DataModel/PrismInstance.h"
#include "V8DataModel/PyramidInstance.h"
#include "V8DataModel/Handles.h"
#include "V8DataModel/GuiObject.h"
#include "Reflection/EnumConverter.h"

namespace WBX {
namespace Reflection {
template<>
EnumDesc<wbx::BasicPartInstance::LegacyPartType>::EnumDesc()
:EnumDescriptor("PartType")
{
	addPair(wbx::BasicPartInstance::BALL_LEGACY_PART, "Ball");
	addPair(wbx::BasicPartInstance::BLOCK_LEGACY_PART, "Block");
	addPair(wbx::BasicPartInstance::CYLINDER_LEGACY_PART, "Cylinder");
}

template<>
EnumDesc<wbx::ExtrudedPartInstance::VisualTrussStyle>::EnumDesc()
:EnumDescriptor("Style")
{
	addPair(wbx::ExtrudedPartInstance::FULL_ALTERNATING_CROSS_BEAM, "AlternatingSupports");
	addPair(wbx::ExtrudedPartInstance::BRIDGE_STYLE_CROSS_BEAM, "BridgeStyleSupports");
	addPair(wbx::ExtrudedPartInstance::NO_CROSS_BEAM, "NoSupports");
	addLegacyName("Alternating Supports", wbx::ExtrudedPartInstance::FULL_ALTERNATING_CROSS_BEAM);
	addLegacyName("Bridge Style Supports", wbx::ExtrudedPartInstance::BRIDGE_STYLE_CROSS_BEAM);
	addLegacyName("No Supports", wbx::ExtrudedPartInstance::NO_CROSS_BEAM);
}

#ifdef _PRISM_PYRAMID_
template<>
EnumDesc<wbx::PrismInstance::NumSidesEnum>::EnumDesc()
:EnumDescriptor("PrismSides")
{
	addPair(wbx::PrismInstance::sides3, "3");
	// Don't allow a 4 sided prism - should use block.
	addPair(wbx::PrismInstance::sides5, "5");
	addPair(wbx::PrismInstance::sides6, "6");
	addPair(wbx::PrismInstance::sides8, "8");
	addPair(wbx::PrismInstance::sides10, "10");
	addPair(wbx::PrismInstance::sides20, "20");
}

template<>
EnumDesc<wbx::PyramidInstance::NumSidesEnum>::EnumDesc()
:EnumDescriptor("PyramidSides")
{
	addPair(wbx::PyramidInstance::sides3, "3");
	addPair(wbx::PyramidInstance::sides4, "4");
	addPair(wbx::PyramidInstance::sides5, "5");
	addPair(wbx::PyramidInstance::sides6, "6");
	addPair(wbx::PyramidInstance::sides8, "8");
	addPair(wbx::PyramidInstance::sides10, "10");
	addPair(wbx::PyramidInstance::sides20, "20");
}
#endif //_PRISM_PYRAMID_

template<>
EnumDesc<wbx::Handles::VisualStyle>::EnumDesc()
:EnumDescriptor("HandlesStyle")
{
	addPair(wbx::Handles::RESIZE_HANDLES, "Resize");
	addPair(wbx::Handles::MOVEMENT_HANDLES, "Movement");
}

template<>
EnumDesc<wbx::GuiObject::SizeConstraint>::EnumDesc()
:EnumDescriptor("SizeConstraint")
{
	addPair(wbx::GuiObject::RELATIVE_XY, "RelativeXY");
	addPair(wbx::GuiObject::RELATIVE_XX, "RelativeXX");
	addPair(wbx::GuiObject::RELATIVE_YY, "RelativeYY");
}

template<>
EnumDesc<wbx::GuiObject::ImageScale>::EnumDesc()
:EnumDescriptor("ScaleType")
{
	addPair(wbx::GuiObject::SCALE_STRETCH, "Stretch");
	addPair(wbx::GuiObject::SCALE_SLICED, "Slice");
}
}//namespace Reflection
}//namespace WBX