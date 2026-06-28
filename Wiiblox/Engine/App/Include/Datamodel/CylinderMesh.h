
#pragma once

#include "BevelMesh.h"

namespace WBX
{
	extern const char* const sCylinderMesh;
	class CylinderMesh 
		: public DescribedCreatable<CylinderMesh, BevelMesh, sCylinderMesh>
	{
	public:
		CylinderMesh(){}
	};
}