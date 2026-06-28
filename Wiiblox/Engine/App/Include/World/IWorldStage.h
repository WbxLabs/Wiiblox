 /* Copyright 2003-2005 Wiiblox Corporation, All Rights Reserved */

#pragma once

#include "V8Kernel/IStage.h"
#include "wbx/Debug.h"
#include "Util/G3DCore.h"

namespace WBX {

	class World;
	class Edge;
	class Contact;
	class Primitive;

	class RBXBaseClass IWorldStage : public IStage {
	private:
		World* world;

	public:
		typedef enum {	NUM_CONTACTSTAGE_CONTACTS,
						NUM_STEPPING_CONTACTS,
						NUM_TOUCHING_CONTACTS,
						MAX_TREE_DEPTH		} MetricType;

		IWorldStage(IStage* upstream, IStage* downstream, World* world) 
			: IStage(upstream, downstream) 
			, world(world)
		{}

		IWorldStage* getUpstreamWS()				{return WBX_static_cast<IWorldStage*>(getUpstream());}
		IWorldStage* getDownstreamWS()				{return WBX_static_cast<IWorldStage*>(getDownstream());}
		const IWorldStage* getDownstreamWS() const	{return WBX_static_cast<const IWorldStage*>(getDownstream());}

		World* getWorld() {return world;}

		////////////////////////////////////////////
		//
		// Calls to DOWNSTREAM stage
		virtual void onEdgeAdded(Edge* e);
		virtual void onEdgeRemoving(Edge* e);

		virtual int getMetric(MetricType metricType) {
			RBXASSERT(getDownstreamWS());
			return getDownstreamWS()->getMetric(metricType);
		}
	};
}	// namespace