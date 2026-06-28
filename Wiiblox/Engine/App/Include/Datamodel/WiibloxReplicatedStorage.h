#pragma once

#include "V8Tree/Service.h"

namespace WBX {

	extern const char* const sWiibloxReplicatedStorage;

	class WiibloxReplicatedStorage
		: public DescribedCreatable<WiibloxReplicatedStorage, Instance, sWiibloxReplicatedStorage, Reflection::ClassDescriptor::INTERNAL, Security::WiibloxScript>
		, public Service
	{
	public:
		WiibloxReplicatedStorage();
	};

}
