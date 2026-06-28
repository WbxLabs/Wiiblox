#include "stdafx.h"

#include "util/wbxrandom.h"

namespace WBX {
	unsigned int randomSeed()
	{
		std::string guid;
		wbx::Guid::generateStandardGUID(guid);
		unsigned int seed = 0;
		for (unsigned int i=0; i < guid.size() - 4; i += 4)
		{
			std::string section = guid.substr(i, 4);
			unsigned int seedSection;
			strncpy((char*) &seedSection, section.c_str(), 4);
			seed ^= seedSection;
		}
		return seed;
	}
}