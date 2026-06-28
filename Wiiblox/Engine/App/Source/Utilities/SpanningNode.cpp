#include "stdafx.h"

#include "Util/SpanningNode.h"
#include "Util/SpanningEdge.h"


namespace WBX {


// Having a parent edge == in the spanning tree

void SpanningNode::setEdgeToParent(SpanningEdge* edge)
{
	edgeToParent = edge;
}

bool SpanningNode::lessThan(const IndexedTree* other) const
{
	WBXASSERT(edgeToParent);
	const SpanningNode* otherNode = WBX_static_cast<const SpanningNode*>(other);
	WBXASSERT(otherNode->getConstEdgeToParent());
	return edgeToParent->isLighterThan(otherNode->getConstEdgeToParent());
}


} // namespace
