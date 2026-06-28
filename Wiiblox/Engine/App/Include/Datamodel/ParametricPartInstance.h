
#pragma once

#include "V8DataModel/BasicPartInstance.h"

namespace WBX {

namespace PART {

	class ParametricPartInstance 
		: public FormFactorPart
	{
	public:
		ParametricPartInstance();
		~ParametricPartInstance();
	};


	///////////////////////////////////////////////////////////////


	extern const char* const sWedge;

	class Wedge
		: public DescribedCreatable<Wedge, ParametricPartInstance, sWedge>
	{
	private:
		// override
		virtual PartType getPartType() const { return WEDGE_PART; }
	public:
		Wedge();
		~Wedge();
	};


}} // namespace