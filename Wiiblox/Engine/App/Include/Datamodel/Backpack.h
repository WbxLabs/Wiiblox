
#pragma once

#include "V8DataModel/Hopper.h"
#include "Script/IScriptFilter.h"

namespace WBX {

	extern const char *const sBackpack;
	class Backpack 
		: public DescribedCreatable<Backpack, Hopper, sBackpack>
		, public IScriptFilter
	{
	private:
		// IScriptOwner
		/*override*/ bool scriptShouldRun(BaseScript* script);

	public:
		Backpack();
	};


}	// namespace 