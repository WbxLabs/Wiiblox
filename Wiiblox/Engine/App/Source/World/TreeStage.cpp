#include "stdafx.h"

#include "V8World/TreeStage.h"
#include "V8World/MovingStage.h"
#include "V8World/SleepStage.h"
#include "V8World/Primitive.h"
#include "V8World/RigidJoint.h"
#include "V8World/MotorJoint.h"
#include "V8World/Clump.h"
#include "V8World/Assembly.h"
#include "V8World/Mechanism.h"
#include "V8Kernel/Body.h"
#include "wbx/Debug.h"

#include <map>

namespace WBX {

///////////////////////////////////////////////////////////////////////////////////

#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : used in base member initializer list
TreeStage::TreeStage(IStage* upstream, World* world)
	: IWorldStage(	upstream, 
					new MovingStage(this, world), 
					world)
	, maxTreeDepth(0)
{}
#pragma warning(pop)


TreeStage::~TreeStage()
{
	WBXASSERT(dirtyMechanisms.size() == 0);
	WBXASSERT(downstreamMechanisms.size() == 0);
}

///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

bool TreeStage::validateTree(SpanningNode* root)
{
	WBXASSERT_NOT_RELEASE();
#ifdef _DEBUG

	if (!Super::validateTree(root)) {
		return false;
	}
	Primitive* primitive = WBX_static_cast<Primitive*>(root);

	Joint* joint = WBX_static_cast<Joint*>(primitive->getEdgeToParent());
	Primitive* parent = primitive->getTypedParent<Primitive>();

	Clump* parentClump = parent ? parent->getClump() : NULL;
	Clump* childClump = primitive->getClump();
	Assembly* childAssembly = primitive->getAssembly();
	Mechanism* childMechanism = primitive->getMechanism();

	WBXASSERT(childClump && childAssembly && childMechanism);

	bool isRigid = RigidJoint::isRigidJoint(joint);
	bool isMotor = Joint::isMotorJoint(joint);
	bool isSpring = Joint::isSpringJoint(joint);
	bool isGround = Joint::isGroundJoint(joint);
	bool isKinematic = (isRigid || isMotor);
	WBXASSERT(isKinematic || isSpring || isGround);
	WBXASSERT(isKinematic == Joint::isKinematicJoint(joint));

	WBXASSERT(isRigid == (parentClump == childClump));
	WBXASSERT(isRigid == (!Clump::isClumpRootPrimitive(primitive)));
	WBXASSERT(isRigid == (primitive->getTypedUpper<Clump>() == NULL));
	WBXASSERT(isKinematic == (!Assembly::isAssemblyRootPrimitive(primitive)));
	WBXASSERT(isGround == (Mechanism::isMechanismRootPrimitive(primitive)));
	WBXASSERT(isKinematic || (primitive->getBody()->getParent() == NULL));
	WBXASSERT(!isKinematic  || (primitive->getBody()->getParent() == parent->getBody()));

	for (int i = 0; i < primitive->numChildren(); ++i)
	{
		Primitive* child = primitive->getTypedChild<Primitive>(i);
		WBXASSERT(child->getTypedParent<Primitive>() == primitive);
		validateTree(child);
	}
#endif
	return true;
}


///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// walk both sides up the tree to a common node, finding the lightest joint along the way
// Parent side is the opposite side from where we found the lightest
// 
// existingActiveJoint will be the case where a freeJoint is added to an existing anchored primitive,
// or an anchorJoint is added to existing free primitive.  In the first case and possibly the second, there
// will be an active joint between the primitive and ground.  If active, it must be the lightest candidate



///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

bool chainToGround(Primitive* p)
{
	if (!p) {
		return true;
	}
	else {
		Primitive* root = p->getRoot<Primitive>();
		Joint* joint = WBX_static_cast<Joint*>(root->getEdgeToParent());
		return (joint && Joint::isGroundJoint(joint));
	}
}

void TreeStage::onSpanningEdgeAdding(SpanningEdge* edge, SpanningNode* child)
{
#ifdef _DEBUG
	Primitive* childPrim = WBX_static_cast<Primitive*>(child);
	WBXASSERT(!childPrim->getTypedUpper<Clump>());
	WBXASSERT(!childPrim->getClump());
	WBXASSERT(!childPrim->getAssembly());
	WBXASSERT(!childPrim->getMechanism());
	WBXASSERT(!chainToGround(childPrim));
#endif
	
	Primitive* parentPrim = WBX_static_cast<Primitive*>(edge->otherNode(child));
	WBXASSERT(!parentPrim || parentPrim->getClump());
	WBXASSERT(!parentPrim || parentPrim->getAssembly());
	WBXASSERT(!parentPrim || parentPrim->getMechanism());
	WBXASSERT(!parentPrim || chainToGround(parentPrim));

	if (parentPrim)
	{
		dirtyMechanism(parentPrim->getMechanism());
	}
}


/*
	Ground:		New Mechanism
	Spring:		New Assembly
	Motor:		New Clump
	Rigid:		.....
*/

void TreeStage::onSpanningEdgeAdded(SpanningEdge* edge)
{
	Joint* joint = WBX_static_cast<Joint*>(edge);
	bool isGroundJoint = Joint::isGroundJoint(joint);
	Primitive* parent = WBX_static_cast<Primitive*>(edge->getParentSpanningNode());
	Primitive* child = WBX_static_cast<Primitive*>(edge->getChildSpanningNode());

	WBXASSERT(chainToGround(child));
	WBXASSERT(child->getBody()->getParent() == NULL);
	WBXASSERT(!child->getTypedUpper<Clump>());
	WBXASSERT(isGroundJoint == (parent == NULL));	
	WBXASSERT(isGroundJoint == (child->getTypedParent<Primitive>() == NULL));

	if (parent)
	{
		dirtyMechanism(parent->getMechanism());
	}

	if (RigidJoint::isRigidJoint(joint)) {						// RIGID JOINT - same clump
		RigidJoint* r = WBX_static_cast<RigidJoint*>(joint);
		child->getBody()->setParent(parent ? parent->getBody() : NULL);
		child->getBody()->setMeInParent(r->getChildInParent(parent, child));
	}
	else {
#ifdef _DEBUG
		Clump* parentClump = parent ? parent->getClump() : NULL;
		WBXASSERT(isGroundJoint == (parentClump == NULL));
#endif
		Clump* childClump = new Clump();
		child->setUpper(childClump);
		if (Joint::isMotorJoint(joint)) {					// MOTOR Joint - same assembly (new clump)

#ifndef WBX_PLATFORM_IOS
			WBXASSERT(parent);
#endif

#pragma warning(push)
#pragma warning(disable:6011)	
			child->getBody()->setParent(parent->getBody());
#pragma warning(pop)
			child->getBody()->setMeInParent( joint->resetLink() );
		}
		else {
#ifdef _DEBUG
			Assembly* parentAssembly = parent ? parent->getAssembly() : NULL;
			WBXASSERT(isGroundJoint == (parentAssembly == NULL));
#endif
			Assembly* childAssembly = new Assembly();
			childClump->setUpper(childAssembly);
			if (!isGroundJoint) {								// Spring Joint - same mechanism (new assembly)
				WBXASSERT(Joint::isSpringJoint(joint));		
			}
			else {
				Mechanism* childMechanism = new Mechanism();	// Ground Joint - new mechanism
				childAssembly->setUpper(childMechanism);
				WBXASSERT(childMechanism->getIndexedMeshParent() == NULL);

				dirtyMechanism(childMechanism);
			}
#ifdef _DEBUG
			WBXASSERT(childAssembly->getIndexedMeshParent() == parentAssembly);
#endif
		}
#ifdef _DEBUG
		WBXASSERT(childClump->getIndexedMeshParent() == parentClump);
#endif
	}

	sendClumpChangedMessage(child);

	WBXASSERT(child->getClump());
	WBXASSERT(child->getAssembly());
	WBXASSERT(child->getMechanism());
}

void assertNotInPipeline(Assembly* a)
{
	WBXASSERT(!a->inPipeline());
}

bool noAssembliesInPipeline(Mechanism* m)
{
	Assembly* a = m->getTypedLower<Assembly>();
	a->visitAssemblies(boost::bind(&assertNotInPipeline, _1));
	return true;
}		

void TreeStage::onSpanningEdgeRemoving(SpanningEdge* edge)
{
	Primitive* child = WBX_static_cast<Primitive*>(edge->getChildSpanningNode());
#ifdef _DEBUG
	Primitive* parent = WBX_static_cast<Primitive*>(edge->getParentSpanningNode());
    WBX_UNUSED(parent);
    WBXASSERT_SLOW(chainToGround(parent));
	WBXASSERT_SLOW(chainToGround(child));
	WBXASSERT(child->getClump());
	WBXASSERT(child->getAssembly());
	WBXASSERT(child->getMechanism());
#endif
	dirtyMechanism(child->getMechanism());
}


void TreeStage::onSpanningEdgeRemoved(SpanningEdge* edge, SpanningNode* childNode)
{
	Joint* joint = WBX_static_cast<Joint*>(edge);
	Primitive* childPrim = WBX_static_cast<Primitive*>(childNode);
#ifdef _DEBUG
	Primitive* parentPrim = WBX_static_cast<Primitive*>(edge->otherNode(childNode));
	WBXASSERT_SLOW(chainToGround(parentPrim));
	WBXASSERT_SLOW(!chainToGround(childPrim));
	WBXASSERT(Joint::isKinematicJoint(joint) == (childPrim->getBody()->getParent() != NULL));
#endif

	if (RigidJoint::isRigidJoint(joint)) 
	{
#ifdef _DEBUG
		WBXASSERT(parentPrim->getClump());
#endif
	}
	else if (Joint::isMotorJoint(joint)) 
	{
		destroyClump(childPrim);
	}
	else if (Joint::isSpringJoint(joint)) 
	{
		destroyAssembly(childPrim);
	}
	else if (Joint::isGroundJoint(joint)) 	// anchor or free joint
	{
		WBXASSERT(childPrim->getAssembly() == childPrim->getClump()->getTypedUpper<Assembly>());
		WBXASSERT(!childPrim->getParent());
#ifdef _DEBUG
		WBXASSERT(!parentPrim);
#endif
		destroyMechanism(childPrim);
	}
	else 
	{
		WBXASSERT(0);
	}

	childPrim->getBody()->setParent(NULL);

	sendClumpChangedMessage(childPrim);

	WBXASSERT(!childPrim->getTypedUpper<Clump>());
	WBXASSERT(!childPrim->getClump());
	WBXASSERT(!childPrim->getAssembly());
	WBXASSERT(!childPrim->getMechanism());
}


void TreeStage::sendClumpChangedMessage(Primitive* childPrim)
{
	if (childPrim->getOwner()) {
		childPrim->getOwner()->onClumpChanged();
	}
	else {
		// Ground Primitive
	}

	// only do the clump
	for (int i = 0; i < childPrim->numChildren(); ++i) {
		Primitive* childChild = childPrim->getTypedChild<Primitive>(i);
		Joint* joint = WBX_static_cast<Joint*>(childChild->getEdgeToParent());
		if (RigidJoint::isRigidJoint(joint)) {
			sendClumpChangedMessage(childChild);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

void TreeStage::removeFromPipeline(Mechanism* m)
{
	if (m->inPipeline()) {
		if (m->downstreamOfStage(this)) {
			WBX_static_cast<MovingStage*>(getDownstreamWS())->onMechanismRemoving(m);
			int num = downstreamMechanisms.erase(m);
			WBXASSERT(num == 1);
		}
		m->removeFromPipeline(this);
	}
	WBXASSERT(noAssembliesInPipeline(m));
}

void TreeStage::dirtyMechanism(Mechanism* m)
{
	WBXASSERT(m);
	removeFromPipeline(m);
	dirtyMechanisms.insert(m);
}

void TreeStage::destroyClump(Primitive* p)
{
	Clump* c = p->getClump();
	p->setUpper(NULL);
	delete c;
}

void TreeStage::destroyAssembly(Primitive* p)
{
	Assembly* a = p->getAssembly();
	Clump* c = p->getClump();

	c->setUpper(NULL);
	p->setUpper(NULL);

	delete c;
	delete a;
}

void TreeStage::destroyMechanism(Primitive* p)
{
	Mechanism* m = p->getMechanism();
	Assembly* a = p->getAssembly();
	Clump* c = p->getClump();

	removeFromPipeline(m);
	dirtyMechanisms.erase(m);

	a->setUpper(NULL);
	c->setUpper(NULL);
	p->setUpper(NULL);

	delete a;
	delete c;
	delete m;
}


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void TreeStage::cleanMechanism(Mechanism* m)
{
	WBXASSERT(!m->getIndexedMeshParent());

	if (!m->inPipeline()) {
		m->putInPipeline(this);
	}

	WBXASSERT(m->inStage(this));
	WBX_static_cast<MovingStage*>(getDownstreamWS())->onMechanismAdded(m);
	bool ok = downstreamMechanisms.insert(m).second;
	WBXASSERT(ok);
}


void TreeStage::assemble()
{
	if (!isAssembled()) 
	{
		// 1. Assemblies
		std::set<Mechanism*>::iterator aIt;
		for (aIt = dirtyMechanisms.begin(); aIt != dirtyMechanisms.end(); ++aIt) {
			cleanMechanism(*aIt);				// clean upstream dirtyAssemblies
		}
		dirtyMechanisms.clear();
	}

	WBXASSERT(isAssembled());
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void TreeStage::onEdgeAdded(Edge* e)
{
	WBXASSERT(e->getPrimitive(0)->inOrDownstreamOfStage(this));
	WBXASSERT(!e->getPrimitive(1) || e->getPrimitive(1)->inOrDownstreamOfStage(this));

	e->putInStage(this);

	if (Joint::isSpanningTreeJoint(e)) {
		Joint* j = WBX_static_cast<Joint*>(e);
		insertSpanningTreeEdge(j);
		if (!(RigidJoint::isRigidJoint(j) || Joint::isGroundJoint(j))) {
			getDownstreamWS()->onEdgeAdded(e);
		}
	}
	else {
		getDownstreamWS()->onEdgeAdded(e);
	}
}


void TreeStage::onEdgeRemoving(Edge* e)
{
	WBXASSERT(e->getPrimitive(0)->inOrDownstreamOfStage(this));
	WBXASSERT(!e->getPrimitive(1) || e->getPrimitive(1)->inOrDownstreamOfStage(this));

	if (Joint::isSpanningTreeJoint(e)) {
		Joint* j = WBX_static_cast<Joint*>(e);
		if (!(RigidJoint::isRigidJoint(j) || Joint::isGroundJoint(j))) {
			getDownstreamWS()->onEdgeRemoving(e);
		}
		if (j->inSpanningTree()) {
			removeSpanningTreeEdge(j);
		}
	}
	else {
		getDownstreamWS()->onEdgeRemoving(e);
	}

	e->removeFromStage(this);
}


void TreeStage::onPrimitiveAdded(Primitive* p)
{
	WBXASSERT(p->getNumEdges() == 0);
	p->putInStage(this);
}


void TreeStage::onPrimitiveRemoving(Primitive* p)
{
	p->removeFromStage(this);
	WBXASSERT(p->getNumEdges() == 0);
}

int TreeStage::getMetric(IWorldStage::MetricType metricType)
{
	switch (metricType)
	{
	case MAX_TREE_DEPTH:
		{
			return maxTreeDepth;
		}
	default:
		{
			return IWorldStage::getMetric(metricType);
		}
	}
}

} // namespace
