#include "stdafx.h"

#include "V8DataModel/NonReplicatedCSGDictionaryService.h"

#include "V8DataModel/CSGDictionaryService.h"
#include "../App/include/v8datamodel/PartOperation.h"
#include "v8datamodel/Value.h"
#include "Util/WiibloxGoogleAnalytics.h"

FASTFLAGVARIABLE(IgnoreBlankDataOnStore, true)

FASTFLAG(StudioCSGAssets)

namespace WBX
{
	const char *const sNonReplicatedCSGDictionaryService = "NonReplicatedCSGDictionaryService";

	NonReplicatedCSGDictionaryService::NonReplicatedCSGDictionaryService()
	{
		setName("NonReplicatedCSGDictionaryService");
	}
	
	void NonReplicatedCSGDictionaryService::reparentChildData(shared_ptr<wbx::Instance> childInstance)
	{
		if (!isChildData(childInstance))
			return;

		CSGDictionaryService* dictionaryService = ServiceProvider::create<CSGDictionaryService>(DataModel::get(this));
		childInstance->setParent(dictionaryService);

		if (shared_ptr<wbx::BinaryStringValue> bStrValue = wbx::Instance::fastSharedDynamicCast<wbx::BinaryStringValue>(childInstance))
		{
			std::string key = createHashKey(bStrValue->getValue().value());
			instanceMap.erase(key);
		}
	}

	void NonReplicatedCSGDictionaryService::storeData(PartOperation& partOperation, bool forceIncrement)
	{
		if (FFlag::StudioCSGAssets)
		{
			BinaryString tmpString = partOperation.getChildData();
			if ( FFlag::IgnoreBlankDataOnStore && tmpString.value().size() > 0 )
			{
				storeStringData(tmpString, forceIncrement, "ChildData");
				partOperation.setChildData(tmpString);
			}
		}
		else
		{
			BinaryString tmpString = partOperation.getChildData();
			storeStringData(tmpString, forceIncrement, "ChildData");
			partOperation.setChildData(tmpString);
		}
	}

	void NonReplicatedCSGDictionaryService::retrieveData(PartOperation& partOperation)
	{
		if (FFlag::StudioCSGAssets)
		{
			BinaryString tmpString = partOperation.getChildData();
			retrieveStringData(tmpString);
			partOperation.setChildData(tmpString);
		}
		else
		{
			BinaryString tmpString = partOperation.getChildData();
			retrieveStringData(tmpString);
			partOperation.setChildData(tmpString);
		}
	}

	void NonReplicatedCSGDictionaryService::storeAllDescendants(shared_ptr<Instance> instance)
	{
		if (instance->getChildren())
			for (wbx::Instances::const_iterator iter = instance->getChildren()->begin(); iter != instance->getChildren()->end(); ++iter)
				storeAllDescendants(*iter);

		if (shared_ptr<PartOperation> childOperation = wbx::Instance::fastSharedDynamicCast<PartOperation>(instance))
			storeData(*childOperation);
	}

	void NonReplicatedCSGDictionaryService::retrieveAllDescendants(shared_ptr<Instance> instance)
	{
		if (instance->getChildren())
			for (wbx::Instances::const_iterator iter = instance->getChildren()->begin(); iter != instance->getChildren()->end(); ++iter)
				retrieveAllDescendants(*iter);

		if (shared_ptr<PartOperation> childOperation = wbx::Instance::fastSharedDynamicCast<PartOperation>(instance))
			retrieveData(*childOperation);
	}

	void NonReplicatedCSGDictionaryService::refreshRefCountUnderInstance(wbx::Instance* instance)
	{
		if (wbx::PartOperation* partOperation = wbx::Instance::fastDynamicCast<wbx::PartOperation>(instance))
			storeData(*partOperation, true);

		if (instance->getChildren())
			for (wbx::Instances::const_iterator iter = instance->getChildren()->begin(); iter != instance->getChildren()->end(); ++iter)
				refreshRefCountUnderInstance(iter->get());
	}
}
