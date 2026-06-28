#include "stdafx.h"

#include "V8DataModel/MouseCommand.h"
#include "V8DataModel/Workspace.h"
#include "V8DataModel/PartInstance.h"
#include "V8DataModel/Camera.h"
#include "V8DataModel/Filters.h"
#include "Humanoid/Humanoid.h"
#include "V8World/Primitive.h"
#include "V8World/ContactManager.h"
#include "V8World/World.h"

namespace WBX {

Vector3 MouseCommand::ignoreVector3;
bool MouseCommand::advArrowToolEnabled = false;//should be false by default

MouseCommand::MouseCommand(Workspace* workspace) :
	workspace(workspace),
	capturedMouse(false),
	rightMouseClickPart(),
	lastHoverPart()
{
}

MouseCommand::~MouseCommand()
{
}

Instance* MouseCommand::getTopSelectable3d(PartInstance* part) const
{
	WBXASSERT(part);

	// do not allow selection of a part that is not selectable
	if( !part->isSelectable3d() ) return NULL;

	Instance* answer = part;
	Instance* temp = part->getParent();
	while ( temp && temp != workspace )
	{
		Selectable* selectable = dynamic_cast<Selectable*>(temp);
		answer = selectable && selectable->isSelectable3d() ?
			temp : answer;
		temp = temp->getParent();
	}
	return answer;
}

TextureId MouseCommand::getCursorId() const
{
	std::string asset = "Textures/" + getCursorName() + ".png";
	if (getCursorName() == "StudsCursor" ||
		getCursorName() == "InletCursor" ||
		getCursorName() == "UniversalCursor") {
		asset = "Textures/FlatCursor.png";
	}
	return ContentId::fromAssets(asset.c_str());
}

void MouseCommand::capture() 
{
	WBXASSERT(!workspace->getCurrentMouseCommand()->captured());
	capturedMouse = true;
}

float MouseCommand::distanceToCharacter(const Vector3& hitPoint) const
{
	if (const PartInstance* head = Humanoid::getConstLocalHeadFromContext(workspace)) {
		Vector3 delta = head->getCoordinateFrame().translation - hitPoint;
		return delta.magnitude();
	}
	return 0;
}

bool MouseCommand::characterCanReach(const Vector3& hitPoint) const
{
	return (Humanoid::getConstLocalHeadFromContext(workspace) && distanceToCharacter(hitPoint) < 60.0);
}

Surface MouseCommand::getSurface(Workspace* workspace, const shared_ptr<InputObject>& inputObject, const HitTestFilter* filter,
								   PartInstance*& part, 
								   int& surfaceId)
{
	if ((part = getPart(workspace, inputObject, filter))) {
		wbx::WBXRay gridRay = getUnitMouseRay(inputObject, workspace);
		return part->getSurface(gridRay, surfaceId);
	}
	return Surface();
}


Surface MouseCommand::getSurface(Workspace* workspace, const shared_ptr<InputObject>& inputObject, const HitTestFilter* filter)
{
	PartInstance* part;
	int surfaceId;
	return getSurface(workspace, inputObject, filter, part, surfaceId);
}

WBXRay MouseCommand::getUnitMouseRay(const shared_ptr<InputObject>& inputObject, ICameraOwner* _workspace)
{
	return _workspace->getConstCamera()->worldRay( inputObject->getRawPosition().x, inputObject->getRawPosition().y);
}

WBXRay MouseCommand::getSearchRay(const WBXRay& unitRay)
{
	WBXASSERT(unitRay.direction().isUnit());

	return WBXRay::fromOriginAndDirection(unitRay.origin(), unitRay.direction() * maxSearch());
}


WBXRay MouseCommand::getSearchRay(const shared_ptr<InputObject>& inputObject, ICameraOwner* _workspace)
{
	WBXRay unitRay = getUnitMouseRay(inputObject, _workspace);

	return getSearchRay(unitRay);
}

WBXRay MouseCommand::getUnitMouseRay(const shared_ptr<InputObject>& inputObject) const 
{
	return getUnitMouseRay(inputObject, workspace);
}

WBXRay MouseCommand::getSearchRay(const shared_ptr<InputObject>& inputObject) const	
{
	return getSearchRay(inputObject, workspace);
}


///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getPartByLocalCharacter(Workspace* workspace, const shared_ptr<InputObject>& inputObject, const HitTestFilter* filter, Vector3& hitWorld)
{
    PartByLocalCharacter characterFilter(workspace);
    MergedFilter mergedFilter(&characterFilter, filter);
    return getPart(workspace, inputObject, &mergedFilter, hitWorld);
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getUnlockedPartByLocalCharacter(const shared_ptr<InputObject>& inputObject, Vector3& hitWorld)
{
	UnlockedPartByLocalCharacter filter(workspace);
	return getPart(workspace, inputObject, &filter, hitWorld);
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getUnlockedPart(const shared_ptr<InputObject>& inputObject, Vector3& hitWorld)
{
	PartInstance* part = getPart(workspace, inputObject, NULL, hitWorld);
	if(part && !part->getPartLocked())
		return part;
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getPart(
	Workspace* workspace,
	const shared_ptr<InputObject>& inputObject, 
	const HitTestFilter* filter,
	Vector3& hitWorld)
{
	WBXRay unitRay = getUnitMouseRay(inputObject, workspace);
	if (unitRay.direction().y != unitRay.direction().y)	// fending against #INV camera direction
		return NULL;
    ContactManager* contactManager = NULL;
    if(workspace && workspace->getWorld())
    {
        contactManager = workspace->getWorld()->getContactManager();
    }
    
    if (!contactManager)
        return NULL;
    

	std::vector<const Primitive *> ignorePrims;
	if (Camera *cam = workspace->getCamera())
    {
		if (CameraSubject *sub = cam->getCameraSubject())
        {
			sub->getSelectionIgnorePrimitives(ignorePrims);
		}
	}

	PartInstance* bestPart = getMousePart(	unitRay,
											*contactManager,
											ignorePrims,
											filter,
											hitWorld);

	if (!bestPart)
    {
		hitWorld = unitRay.origin() + 10000.0f * unitRay.direction();
	}
	return bestPart;
}

///////////////////////////////////////////////////////////////////////////////

PartInstance* MouseCommand::getMousePart(
							const WBXRay& unitRay,
							const ContactManager& contactManager,
							const std::vector<const Primitive*> &ignore,
							const HitTestFilter* filter,
							Vector3& hitPoint,
							float maxSearchGrid)
{
	if (!unitRay.direction().isUnit())
		return NULL;
	WBXRay searchRay = WBXRay::fromOriginAndDirection(unitRay.origin(), unitRay.direction() * maxSearch());
	
	Primitive* bestPrimitive = contactManager.getHit(	searchRay,
														&ignore,
														filter,
														hitPoint);

	return bestPrimitive ? PartInstance::fromPrimitive(bestPrimitive) : NULL;
}


PartInstance* MouseCommand::getMousePart(
							const WBXRay& unitRay,
							const ContactManager& contactManager,
							const Primitive* ignore,
							const HitTestFilter* filter,
							Vector3& hitPoint,
							float maxSearchGrid)
{
	std::vector<const Primitive *> ignorePrims;
	ignorePrims.push_back(ignore);
	return getMousePart(unitRay, contactManager, ignorePrims, filter, hitPoint, maxSearchGrid);
}


} // namespace
