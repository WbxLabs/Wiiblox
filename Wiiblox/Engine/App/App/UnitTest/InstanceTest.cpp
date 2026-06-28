#include <boost/test/unit_test.hpp>

#include "v8tree/instance.h"
#include "v8dataModel/BasicPartInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8dataModel/Tool.h"

extern const char* const sInstanceTest;
class InstanceTest : public wbx::DescribedCreatable<InstanceTest, wbx::Instance, sInstanceTest>
{
};

const char* const sInstanceTest = "InstanceTest";
WBX_REGISTER_CLASS(InstanceTest);

BOOST_AUTO_TEST_SUITE(Instance)

	BOOST_AUTO_TEST_CASE(Clone)
	{
		int count = wbx::Diagnostics::Countable<wbx::Instance>::getCount();
		{
			boost::shared_ptr<wbx::Instance> i = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
			i->clone(wbx::ScriptingCreator);
		}
		BOOST_CHECK_EQUAL(count, wbx::Diagnostics::Countable<wbx::Instance>::getCount());
	}

		BOOST_AUTO_TEST_CASE(PerfDynamicCast)
		{
			shared_ptr<wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
			wbx::Instance* inst = part.get();
			for (int i=0; i<1000000; ++i)
			{
				dynamic_cast<wbx::Tool*>(inst);
			}
		}

		BOOST_AUTO_TEST_CASE(PerfIsA)
		{
			shared_ptr<wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
			wbx::Instance* inst = part.get();
			for (int i=0; i<1000000; ++i)
			{
				inst->getDescriptor().isA(wbx::PartInstance::classDescriptor());
			}
		}

		BOOST_AUTO_TEST_CASE(PerfIsAFalse)
		{
			boost::shared_ptr<wbx::Instance> part = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
			wbx::Instance* inst = part.get();
			for (int i=0; i<1000000; ++i)
			{
				inst->isA<wbx::PartInstance>();
			}
		}

		BOOST_AUTO_TEST_CASE(PerfIsATool)
		{
			shared_ptr<wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
			wbx::Instance* inst = part.get();
			for (int i=0; i<1000000; ++i)
			{
				inst->isA<wbx::Tool>();
			}
		}

	BOOST_AUTO_TEST_CASE(CastTo)
	{
		shared_ptr<wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
		shared_ptr<wbx::Instance> instance = wbx::Creatable<wbx::Instance>::create<InstanceTest>();

		BOOST_CHECK(wbx::Instance::fastDynamicCast<wbx::Instance>(part.get()));

		BOOST_CHECK(!wbx::Instance::fastDynamicCast<wbx::PartInstance>(instance.get()));

		wbx::Instance* nullInstance = NULL;
		BOOST_CHECK(!wbx::Instance::fastDynamicCast<wbx::PartInstance>(nullInstance));

		wbx::Instance* inst = part.get();
		BOOST_CHECK(inst->fastDynamicCast<wbx::Instance>());

		BOOST_CHECK(!instance->fastDynamicCast<wbx::PartInstance>());
	}

	BOOST_AUTO_TEST_CASE(CastToConst)
	{
		shared_ptr<const wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
		shared_ptr<const wbx::Instance> instance = wbx::Creatable<wbx::Instance>::create<InstanceTest>();

		BOOST_CHECK(wbx::Instance::fastDynamicCast<wbx::Instance>(part.get()));

		BOOST_CHECK(!wbx::Instance::fastDynamicCast<wbx::PartInstance>(instance.get()));

		const wbx::Instance* nullInstance = NULL;
		BOOST_CHECK(!wbx::Instance::fastDynamicCast<wbx::PartInstance>(nullInstance));

		const wbx::Instance* inst = part.get();
		BOOST_CHECK(inst->fastDynamicCast<wbx::Instance>());

		BOOST_CHECK(!instance->fastDynamicCast<wbx::PartInstance>());
	}

	BOOST_AUTO_TEST_CASE(SharedCastTo)
	{
		shared_ptr<const wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
		shared_ptr<const wbx::Instance> instance = wbx::Creatable<wbx::Instance>::create<InstanceTest>();

		BOOST_CHECK(wbx::Instance::fastSharedDynamicCast<const wbx::Instance>(part));

		BOOST_CHECK(!wbx::Instance::fastSharedDynamicCast<const wbx::PartInstance>(instance));

		shared_ptr<const wbx::PartInstance> nullInstance;
		BOOST_CHECK(!wbx::Instance::fastSharedDynamicCast<const wbx::PartInstance>(nullInstance));
	}

BOOST_AUTO_TEST_SUITE_END()

