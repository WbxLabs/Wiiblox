#include "stdafx.h"

#include "V8World/IWorldStage.h"
#include "V8World/Contact.h"
#include "V8World/Edge.h"

namespace WBX {

void IWorldStage::onEdgeAdded(Edge* e) 
{
	WBXASSERT(getDownstreamWS());
	e->putInStage(this);
	getDownstreamWS()->onEdgeAdded(e);
}

void IWorldStage::onEdgeRemoving(Edge* e) 
{
	WBXASSERT(getDownstreamWS());
	getDownstreamWS()->onEdgeRemoving(e);
	e->removeFromStage(this);
}

} // namespace
