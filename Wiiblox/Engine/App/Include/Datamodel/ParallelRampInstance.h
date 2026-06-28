
#pragma once

#include "V8DataModel/PartInstance.h"
#include "V8DataModel/BasicPartInstance.h"
#include "reflection/reflection.h"

#ifdef _PRISM_PYRAMID_

namespace WBX {

extern const char* const sParallelRamp;

class ParallelRampInstance
	: public DescribedNonCreatable<ParallelRampInstance, PartInstance, sParallelRamp>
{
	public:

		ParallelRampInstance();
		~ParallelRampInstance();
	 
		/*override*/ virtual PartType getPartType() const { return PARALLELRAMP_PART; }


};

} // namespace

#endif // _PRISM_PYRAMID_