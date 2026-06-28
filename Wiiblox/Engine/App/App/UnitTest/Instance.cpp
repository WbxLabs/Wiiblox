#include <boost/test/unit_test.hpp>
#include "wbx/test/Base.UnitTest.Lib.h"
#include "wbx/test/ScopedFastFlagSetting.h"

#include "script/script.h"
#include "v8tree/instance.h"
#include "v8dataModel/BasicPartInstance.h"
#include "v8datamodel/PartInstance.h"
#include "v8dataModel/Tool.h"
#include "wbx/test/DataModelFixture.h"

extern const char* const sInstanceTest;
class InstanceTest : public wbx::DescribedCreatable<InstanceTest, wbx::Instance, sInstanceTest>
{
};

const char* const sInstanceTest = "InstanceTest";
WBX_REGISTER_CLASS(InstanceTest);

extern const char* const sStealDescendants;
class StealDescendants : public wbx::DescribedCreatable<StealDescendants, wbx::Instance, sStealDescendants>
{
private:
	typedef wbx::DescribedCreatable<StealDescendants, wbx::Instance, sStealDescendants> Super;
	/*override*/ void onDescendantAdded(Instance* instance)
	{
		if(instance->getParent() != this)
		{
			instance->setAndLockParent(this);
		}
		Super::onDescendantAdded(instance);
	}
};

const char* const sStealDescendants = "StealDescendants";
WBX_REGISTER_CLASS(StealDescendants);

extern const char* const sThrowOnDescendants;
class ThrowOnDescendants : public wbx::DescribedCreatable<ThrowOnDescendants, wbx::Instance, sThrowOnDescendants>
{
private:
	typedef wbx::DescribedCreatable<ThrowOnDescendants, wbx::Instance, sThrowOnDescendants> Super;
	/*override*/ void onDescendantAdded(Instance* instance)
	{
		if(instance->getParent() != this)
		{
			throw std::runtime_error("");
		}
		Super::onDescendantAdded(instance);
	}
};

const char* const sThrowOnDescendants = "ThrowOnDescendants";
WBX_REGISTER_CLASS(ThrowOnDescendants);

extern const char* const sCheckAncestor;
const char* const sCheckAncestor = "CheckAncestor";

class CheckAncestor
	: public wbx::DescribedCreatable<CheckAncestor, wbx::Instance, sCheckAncestor>
{
private:
	typedef wbx::DescribedCreatable<CheckAncestor, wbx::Instance, sCheckAncestor> Super;

public:
	CheckAncestor() 
		: wbx::DescribedCreatable<CheckAncestor, wbx::Instance, sCheckAncestor>()
	{
		setName(sCheckAncestor);
	}


protected:
	void verifySetAncestor(const wbx::Instance* const newParent, const wbx::Instance* const instanceGettingNewParent) const
	{
		if (wbx::ServiceProvider::findServiceProvider(newParent))
			throw wbx::runtime_error("CheckAncestor can't have a service provider for an ancestor");
		Super::verifySetAncestor(newParent, instanceGettingNewParent);
	}
};

WBX_REGISTER_CLASS(CheckAncestor);

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

	BOOST_FIXTURE_TEST_CASE(PerfDynamicCast, wbx::Test::PerformanceTestFixture)
	{
		shared_ptr<wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
		wbx::Instance* inst = part.get();
		for (int i=0; i<1000000; ++i)
		{
			wbx::Tool* testTool = dynamic_cast<wbx::Tool*>(inst);
            testTool = 0;
		}
	}

	BOOST_FIXTURE_TEST_CASE(PerfIsA, wbx::Test::PerformanceTestFixture)
	{
		shared_ptr<wbx::PartInstance> part = wbx::Creatable<wbx::Instance>::create<wbx::BasicPartInstance>();
		wbx::Instance* inst = part.get();
		for (int i=0; i<1000000; ++i)
		{
			inst->getDescriptor().isA(wbx::PartInstance::classDescriptor());
		}
	}

	BOOST_FIXTURE_TEST_CASE(PerfIsAFalse, wbx::Test::PerformanceTestFixture)
	{
		boost::shared_ptr<wbx::Instance> part = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
		wbx::Instance* inst = part.get();
		for (int i=0; i<1000000; ++i)
		{
			inst->isA<wbx::PartInstance>();
		}
	}

	BOOST_FIXTURE_TEST_CASE(PerfIsATool, wbx::Test::PerformanceTestFixture)
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

	BOOST_FIXTURE_TEST_CASE(LockUnlockDeprecated, DataModelFixture)
	{
		std::auto_ptr<wbx::Reflection::Tuple> result = execute("Instance.Unlock() return Instance.Lock(nil)");
		BOOST_REQUIRE_EQUAL(result->values.size(), 1);
		BOOST_CHECK(result->values.at(0).get<bool>());
	}

	BOOST_AUTO_TEST_CASE(Parenting)
	{
		shared_ptr<wbx::Instance> instance = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
		shared_ptr<wbx::Instance> checkAncestor = wbx::Creatable<wbx::Instance>::create<CheckAncestor>();
		
		DataModelFixture dm;
		wbx::DataModel::LegacyLock lock(&dm, wbx::DataModelJob::Write);

		checkAncestor->setParent2(instance);
		BOOST_CHECK(checkAncestor->getParent() != NULL);
		BOOST_CHECK_THROW(instance->setParent2(dm.dataModel), std::exception);
		instance->setParent(NULL);
		BOOST_CHECK_THROW(checkAncestor->setParent2(dm.dataModel), std::exception);
		checkAncestor->setParent(NULL);
		instance->setParent2(dm.dataModel);
		BOOST_CHECK_THROW(checkAncestor->setParent2(instance), std::exception);
		instance->setParent(NULL);
		checkAncestor->setParent(NULL);
	}

	BOOST_AUTO_TEST_CASE(ReparentingLock)
	{
		shared_ptr<wbx::Instance> thievingGrandparent = wbx::Creatable<wbx::Instance>::create<StealDescendants>();
		shared_ptr<wbx::Instance> throwingGrandparent = wbx::Creatable<wbx::Instance>::create<ThrowOnDescendants>();
		shared_ptr<wbx::Instance> parent = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
		shared_ptr<wbx::Instance> child = wbx::Creatable<wbx::Instance>::create<InstanceTest>();

		BOOST_CHECK(!parent->getIsParentLocked());
		parent->setAndLockParent(thievingGrandparent.get());
		BOOST_CHECK(parent->getIsParentLocked());

		// grandparent will attempt to steal child from parent, child's parent lock should still have its parent locked
		child->setAndLockParent(parent.get());
		BOOST_CHECK(child->getIsParentLocked());
		BOOST_CHECK(child->getParent() == parent.get());

		child->unlockParent();
		child->setParent(NULL);

		// grandparent will attempt to steal child from parent, child should not have it's parent locked
		child->setParent(parent.get());
		BOOST_CHECK(!child->getIsParentLocked());
		BOOST_CHECK(child->getParent() == parent.get());

		child->setParent(NULL);

		parent->unlockParent();
		parent->setParent(throwingGrandparent.get());

		BOOST_CHECK_THROW(child->setAndLockParent(parent.get()), std::exception);
		BOOST_CHECK(!child->getIsParentLocked());
		
		child->setParent(NULL);
		child->lockParent();
		BOOST_CHECK_THROW(child->setAndLockParent(parent.get()), std::exception);
		BOOST_CHECK(child->getIsParentLocked());
	}
	
	BOOST_AUTO_TEST_CASE(SetParentNull)
	{
		shared_ptr<wbx::Instance> parent = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
		
		{
			shared_ptr<wbx::Instance> child = wbx::Creatable<wbx::Instance>::create<InstanceTest>();
			child->setParent(parent.get());
		} // child goes out of scope, but the object is still alive since it's a child of parent now
		
		wbx::Instance* child = parent->getChild(0);
		child->setParent(NULL);
	}

	BOOST_AUTO_TEST_CASE(WaitForChildReturnsImmediatelyIfChildPresent)
	{
		using namespace WBX;

		shared_ptr<Script> script = Creatable<wbx::Instance>::create<Script>();
		script->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('PartA').Name = 'Renamed'"));

		shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);
		
			script->setParent(dm->getWorkspace());

			part->setName("PartA");
			part->setParent(dm->getWorkspace());

			ServiceProvider::create<RunService>(&dm)->run();

			BOOST_CHECK_EQUAL(part->getName(), "Renamed");
		}
	}

	BOOST_AUTO_TEST_CASE(WaitForSameChildAfterInitialWaitForChild)
	{
		using namespace WBX;
		shared_ptr<Script> script = Creatable<wbx::Instance>::create<Script>();
		script->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('PartA') "
			"Workspace:WaitForChild('PartA').Name = 'Renamed'"));

		shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);
		
			script->setParent(dm->getWorkspace());

			part->setName("PartA");
			part->setParent(dm->getWorkspace());

			ServiceProvider::create<RunService>(&dm)->run();

			BOOST_CHECK_EQUAL(part->getName(), "Renamed");
		}
	}

	BOOST_AUTO_TEST_CASE(TwoScriptsWaitingForSameChild)
	{
		using namespace WBX;
		shared_ptr<Script> script1 = Creatable<wbx::Instance>::create<Script>();
		script1->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('PartA') "
			"Instance.new('Part', Workspace).Name = 'Alice'"));

		shared_ptr<Script> script2 = Creatable<wbx::Instance>::create<Script>();
		script2->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('PartA') "
			"Instance.new('Part', Workspace).Name = 'Bob'"));

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			script1->setParent(dm->getWorkspace());
			script2->setParent(dm->getWorkspace());
			ServiceProvider::create<RunService>(&dm)->run();
		}

		G3D::System::sleep(.5f);

		shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			part->setName("PartA");
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("Alice") != NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("Bob") != NULL);
		}
	}

	BOOST_AUTO_TEST_CASE(TwoScriptsWaitingForSameChildRenameOnResume)
	{
		using namespace WBX;
		shared_ptr<Script> script1 = Creatable<wbx::Instance>::create<Script>();
		script1->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('PartA').Name = 'PartB' "));

		shared_ptr<Script> script2 = Creatable<wbx::Instance>::create<Script>();
		script2->setEmbeddedCode(ProtectedString::fromTestSource(
			"wait() "
			"p = Workspace:WaitForChild('PartA') "
			"p.Name = p.Name .. 'x' "));

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			script1->setParent(dm->getWorkspace());
			script2->setParent(dm->getWorkspace());

			ServiceProvider::create<RunService>(&dm)->run();
		}

		G3D::System::sleep(.5f);

		shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			part->setName("PartA");
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK_EQUAL("PartBx", part->getName());
		}
		
		// check for double-resume scenario
		shared_ptr<wbx::Instance> part2 = Creatable<wbx::Instance>::create<BasicPartInstance>();
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			part2->setName("PartA");
			part2->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);
			BOOST_CHECK_EQUAL("PartA", part2->getName());
		}
	}

	BOOST_AUTO_TEST_CASE(TwoScriptsWaitingForDifferentChildren)
	{
		using namespace WBX;
		shared_ptr<Script> script1 = Creatable<wbx::Instance>::create<Script>();
		script1->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('Alice').Name = 'AliceX' "));

		shared_ptr<Script> script2 = Creatable<wbx::Instance>::create<Script>();
		script2->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('Bob').Name = 'BobX' "));

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			script1->setParent(dm->getWorkspace());
			script2->setParent(dm->getWorkspace());

			ServiceProvider::create<RunService>(&dm)->run();
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();
			part->setName("Alice");
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("Alice") == NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("AliceX") != NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("Bob") == NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("BobX") == NULL);
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();
			part->setName("Bob");
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("Alice") == NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("AliceX") != NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("Bob") == NULL);
			BOOST_CHECK(dm->getWorkspace()->findFirstChildByName("BobX") != NULL);
		}
	}

	BOOST_AUTO_TEST_CASE(WaitForDifferentChildAfterInitialWaitForChild)
	{
		using namespace WBX;
		shared_ptr<Script> script = Creatable<wbx::Instance>::create<Script>();
		script->setEmbeddedCode(ProtectedString::fromTestSource(
			"Workspace:WaitForChild('PartA').Name = 'Phase1' "
			"Workspace:WaitForChild('PartB').Name = 'Phase2'"));

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);
		
			script->setParent(dm->getWorkspace());
			ServiceProvider::create<RunService>(&dm)->run();
		}

		G3D::System::sleep(.5f);

		shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			part->setName("PartA");
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK_EQUAL(part->getName(), "Phase1");

			part->setName("PartB");
			part->setParent(NULL);
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);
		
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK_EQUAL(part->getName(), "Phase2");
		}
	}

	BOOST_AUTO_TEST_CASE(WaitForChildOutsideOfDataModel)
	{
		using namespace WBX;

		shared_ptr<Script> script = Creatable<wbx::Instance>::create<Script>();
		script->setEmbeddedCode(ProtectedString::fromTestSource(
			"p = Workspace:WaitForChild('PartA') "
			"p.Parent = nil "
			"p.Name = 'Phase1' "
			"p:WaitForChild('PartB') "
			"p.Name = 'Phase2'"));

		shared_ptr<wbx::Instance> part = Creatable<wbx::Instance>::create<BasicPartInstance>();

		DataModelFixture dm;
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);
		
			script->setParent(dm->getWorkspace());
			ServiceProvider::create<RunService>(&dm)->run();
		}

		G3D::System::sleep(.5f);

		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			part->setName("PartA");
			part->setParent(dm->getWorkspace());
		}

		G3D::System::sleep(.5f);
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK_EQUAL(part->getName(), "Phase1");
			BOOST_REQUIRE(part->getParent() == NULL);

			shared_ptr<wbx::Instance> subPart = Creatable<wbx::Instance>::create<BasicPartInstance>();
			subPart->setName("PartB");
			subPart->setParent(part.get());
		}

		G3D::System::sleep(.5f);
		
		{
			DataModel::LegacyLock l(&dm, DataModelJob::Write);

			BOOST_CHECK_EQUAL(part->getName(), "Phase2");
		}

	}

	BOOST_AUTO_TEST_SUITE_END()

