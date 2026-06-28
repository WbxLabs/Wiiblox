#pragma once

#include "Util/BinaryString.h"
#include "V8Tree/Instance.h"
#include "V8Tree/Service.h"
#include "V8DataModel/FlyweightService.h"

#include <boost/unordered_map.hpp>
#include "Value.h"

namespace WBX
{
	class PartOperation;

	extern const char *const sNonReplicatedCSGDictionaryService;

	class NonReplicatedCSGDictionaryService
		: public DescribedCreatable<NonReplicatedCSGDictionaryService, FlyweightService, sNonReplicatedCSGDictionaryService, Reflection::ClassDescriptor::PERSISTENT, Security::Wiiblox>
	{
	protected:
		typedef DescribedCreatable<NonReplicatedCSGDictionaryService, FlyweightService, sNonReplicatedCSGDictionaryService, Reflection::ClassDescriptor::PERSISTENT, Security::Wiiblox> Super;

		virtual void refreshRefCountUnderInstance(wbx::Instance* instance);

		void reparentChildData(shared_ptr<wbx::Instance> childInstance);

	public:

		NonReplicatedCSGDictionaryService();

		void storeData(PartOperation& partOperation, bool forceIncrement = false);
		void retrieveData(PartOperation& partOperation);

		void storeAllDescendants(shared_ptr<wbx::Instance> instance);
		void retrieveAllDescendants(shared_ptr<wbx::Instance> instance);
	};
}
