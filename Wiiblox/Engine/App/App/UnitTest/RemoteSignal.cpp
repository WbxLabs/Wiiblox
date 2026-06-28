/*
	This unit tests the two flavors of RemoteEventDesc. One with fancy macros and one without.
	Erik is working on a replacement for RemoteEventDesc, so eventually this should be deprecated
	and then factored out.
*/
#include <boost/test/unit_test.hpp>

#include "wbx/test/test_tools.h"

#include "wbx/test/DataModelFixture.h"
#include "v8dataModel/Workspace.h"
#include "v8dataModel/EventReplicator.h"
#include "Network/Players.h"

using namespace WBX;

BOOST_GLOBAL_FIXTURE(NetworkFixture)

extern const char* const sRemoteEventClass;
class RemoteEventClass : public wbx::DescribedCreatable<RemoteEventClass, wbx::Instance, sRemoteEventClass>
{
public:
	wbx::remote_signal<void(int)> signal;

	CEvent evt;
	int lastValue;

	RemoteEventClass():evt(false),lastValue(-1)
	{
	}

	wbx::signals::connection selfConnect()
	{
		return signal.connect(boost::bind(&RemoteEventClass::onEvent, this, _1));
	}

	void onEvent(int value)
	{
		lastValue = value;
		evt.Set();
	}
};

static wbx::Reflection::RemoteEventDesc<RemoteEventClass, void(int)> test_BroadcastEvent(&RemoteEventClass::signal, "BroadcastEvent", "value", Security::None, Reflection::RemoteEventCommon::REPLICATE_ONLY, Reflection::RemoteEventCommon::BROADCAST);

const char* const sRemoteEventClass = "RemoteEventClass";
WBX_REGISTER_CLASS(RemoteEventClass);


extern const char* const sEventReplicatorClass;
class EventReplicatorClass : public wbx::DescribedCreatable<EventReplicatorClass, wbx::Instance, sEventReplicatorClass>
{
	bool listenerModeSet;
public:
	wbx::remote_signal<void(int)> signal;
	DECLARE_EVENT_REPLICATOR_SIG(EventReplicatorClass,Signal, void(int));

	CEvent evt;
	int lastValue;

	EventReplicatorClass();

	wbx::signals::connection selfConnect()
	{
		return signal.connect(boost::bind(&EventReplicatorClass::onEvent, this, _1));
	}

	void onEvent(int value)
	{
		lastValue = value;
		evt.Set();
	}

	void onPropertyChanged(const Reflection::PropertyDescriptor& descriptor)
	{
		wbx::Instance::onPropertyChanged(descriptor);
		eventReplicatorSignal.onPropertyChanged(descriptor);
	}

	void onAncestorChanged(const AncestorChanged& event)
	{
		wbx::Instance::onAncestorChanged(event);

		if(!listenerModeSet && Network::Players::serverIsPresent(this,false))
		{
			listenerModeSet = true;
			eventReplicatorSignal.setListenerMode(!signal.empty());
		}
	}
};

static wbx::Reflection::RemoteEventDesc<EventReplicatorClass, void(int)> test_Event(&EventReplicatorClass::signal, "Event", "value", Security::None, Reflection::RemoteEventCommon::REPLICATE_ONLY, Reflection::RemoteEventCommon::BROADCAST);
IMPLEMENT_EVENT_REPLICATOR(EventReplicatorClass, test_Event, "Event", Signal);

EventReplicatorClass::EventReplicatorClass():evt(false),lastValue(-1),listenerModeSet(false)
	, CONSTRUCT_EVENT_REPLICATOR(EventReplicatorClass,EventReplicatorClass::signal, test_Event, Signal)
	{
		CONNECT_EVENT_REPLICATOR(Signal);
	}

const char* const sEventReplicatorClass = "EventReplicatorClass";
WBX_REGISTER_CLASS(EventReplicatorClass);



BOOST_AUTO_TEST_SUITE(RemoteSignals)

BOOST_AUTO_TEST_CASE(RemoteEvent)
{
	DataModelFixture serverDm;
	NetworkFixture::startServer(serverDm);

	shared_ptr<RemoteEventClass> serverTest;

	// Create a RemoteEventClass on the server
	{
		wbx::DataModel::LegacyLock lock(&serverDm, wbx::DataModelJob::Write);

		serverTest = Creatable<Instance>::create<RemoteEventClass>();
		serverTest->setName("Test");
		serverTest->setParent(serverDm->getWorkspace());
	}

	DataModelFixture clientDm;
	NetworkFixture::startClient(clientDm);

	// Wait for it to replicate
	G3D::System::sleep(2);

	shared_ptr<RemoteEventClass> clientTest;

	// Confirm the client got it
	{
		wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
		clientTest = shared_from_static_cast<RemoteEventClass>(clientDm->getWorkspace()->findFirstChildByName("Test"));
		BOOST_CHECK(clientTest != 0);
	}

	{
		BOOST_MESSAGE("Test Client to Server");

#if !WBX_PLATFORM_IOS
		wbx::signals::scoped_connection connection(serverTest->selfConnect());

		{
			wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
			test_BroadcastEvent.fireAndReplicateEvent(clientTest.get(), 12);
		}

		BOOST_CHECK(serverTest->evt.Wait(wbx::Time::Interval::from_seconds(10)));	
		BOOST_CHECK_EQUAL(serverTest->lastValue, 12);
#endif
	}

	{
		BOOST_MESSAGE("Test Server to Client");

#if !WBX_PLATFORM_IOS
		wbx::signals::scoped_connection connection(clientTest->selfConnect());

		{
			wbx::DataModel::LegacyLock lock(&serverDm, wbx::DataModelJob::Write);
			test_BroadcastEvent.fireAndReplicateEvent(serverTest.get(), 13);
		}

		BOOST_CHECK(clientTest->evt.Wait(wbx::Time::Interval::from_seconds(10)));	
		BOOST_CHECK_EQUAL(clientTest->lastValue, 13);
#endif
	}

	DataModelFixture client2Dm;
	NetworkFixture::startClient(client2Dm);

	// Wait for it to replicate
	G3D::System::sleep(2);

	shared_ptr<RemoteEventClass> client2Test;

	// Confirm the client got it
	{
		wbx::DataModel::LegacyLock lock(&client2Dm, wbx::DataModelJob::Write);
		client2Test = shared_from_static_cast<RemoteEventClass>(client2Dm->getWorkspace()->findFirstChildByName("Test"));
		BOOST_CHECK(client2Test != 0);
	}

	{
		BOOST_MESSAGE("Test Client to Client2");

#if !WBX_PLATFORM_IOS
		wbx::signals::scoped_connection connection(client2Test->selfConnect());

		{
			wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
			test_BroadcastEvent.fireAndReplicateEvent(clientTest.get(), 14);
		}

		BOOST_CHECK(client2Test->evt.Wait(wbx::Time::Interval::from_seconds(10)));	
		BOOST_CHECK_EQUAL(client2Test->lastValue, 14);
#endif
	}


}




BOOST_AUTO_TEST_CASE(EventReplicator)
{
	DataModelFixture serverDm;
	NetworkFixture::startServer(serverDm);

	shared_ptr<EventReplicatorClass> serverTest;

	// Create a EventReplicatorClass on the server
	{
		wbx::DataModel::LegacyLock lock(&serverDm, wbx::DataModelJob::Write);

		serverTest = Creatable<Instance>::create<EventReplicatorClass>();
		serverTest->setName("Test");
		serverTest->setParent(serverDm->getWorkspace());
	}

	DataModelFixture clientDm;
	NetworkFixture::startClient(clientDm);

	// Wait for it to replicate
	G3D::System::sleep(4);

	shared_ptr<EventReplicatorClass> clientTest;

	// Confirm the client got it
	{
		wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
		clientTest = shared_from_static_cast<EventReplicatorClass>(clientDm->getWorkspace()->findFirstChildByName("Test"));
		BOOST_CHECK(clientTest != 0);
	}

	{
		BOOST_MESSAGE("Test Client to Server");

		wbx::signals::scoped_connection connection;
		{
			wbx::DataModel::LegacyLock lock(&serverDm, wbx::DataModelJob::Write);
			connection = serverTest->selfConnect();
		}
		G3D::System::sleep(4);	// wait for connection to replicate

		{
			wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
			clientTest->signal(12);
		}

		BOOST_CHECK(serverTest->evt.Wait(wbx::Time::Interval::from_seconds(10)));	
		BOOST_CHECK_EQUAL(serverTest->lastValue, 12);
	}

	{
		BOOST_MESSAGE("Test Client not to Server");

		G3D::System::sleep(4);	// wait for connection to replicate

		{
			wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
			clientTest->signal(12);
		}

		BOOST_CHECK(!serverTest->evt.Wait(wbx::Time::Interval::from_seconds(4)));	
	}

	{
		BOOST_MESSAGE("Test Server to Client");

		wbx::signals::scoped_connection connection;
		{
			wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
			connection = clientTest->selfConnect();
		}
		G3D::System::sleep(4);	// wait for connection to replicate

		{
			wbx::DataModel::LegacyLock lock(&serverDm, wbx::DataModelJob::Write);
			serverTest->signal(13);
		}

		BOOST_WARN(clientTest->evt.Wait(wbx::Time::Interval::from_seconds(10)));	
		BOOST_WARN_EQUAL(clientTest->lastValue, 13);
	}

	DataModelFixture client2Dm;
	NetworkFixture::startClient(client2Dm);

	// Wait for it to replicate
	G3D::System::sleep(4);

	shared_ptr<EventReplicatorClass> client2Test;

	// Confirm the client got it
	{
		wbx::DataModel::LegacyLock lock(&client2Dm, wbx::DataModelJob::Write);
		client2Test = shared_from_static_cast<EventReplicatorClass>(client2Dm->getWorkspace()->findFirstChildByName("Test"));
		BOOST_CHECK(client2Test != 0);
	}

	{
		BOOST_MESSAGE("Test Client to Client2");

		wbx::signals::scoped_connection connection;
		{
			wbx::DataModel::LegacyLock lock(&client2Dm, wbx::DataModelJob::Write);
			connection = client2Test->selfConnect();
		}
		G3D::System::sleep(4);	// wait for connection to replicate

		{
			wbx::DataModel::LegacyLock lock(&clientDm, wbx::DataModelJob::Write);
			clientTest->signal(14);
		}

		BOOST_CHECK(client2Test->evt.Wait(wbx::Time::Interval::from_seconds(10)));	
		BOOST_CHECK_EQUAL(client2Test->lastValue, 14);
	}


}




BOOST_AUTO_TEST_SUITE_END()

