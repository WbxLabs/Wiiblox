#include "stdafx.h"

#include "Util/KeywordFilter.h"
#include "Reflection/EnumConverter.h"

namespace WBX 
{
	namespace Reflection
	{
		template<>
		Reflection::EnumDesc<KeywordFilterType>::EnumDesc()
			:wbx::Reflection::EnumDescriptor("KeywordFilterType")
		{
			addPair(INCLUDE_KEYWORDS,"Include");
			addPair(EXCLUDE_KEYWORDS,"Exclude");
		}
	}
}
