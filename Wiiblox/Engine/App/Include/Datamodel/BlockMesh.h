
#pragma once

#include "BevelMesh.h"

namespace WBX
{
	extern const char* const sBlockMesh;
	class BlockMesh 
		: public DescribedCreatable<BlockMesh, BevelMesh, sBlockMesh>
	{
	public:
		BlockMesh(){}
	};
}