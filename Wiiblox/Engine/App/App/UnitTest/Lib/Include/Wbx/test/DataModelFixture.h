#pragma once

// Helpful utility classes you can use to do DataModel-driven tests

#include "v8datamodel/datamodel.h"
#include "v8datamodel/factoryregistration.h"
#include "v8datamodel/GameSettings.h"
#include "v8datamodel/DebugSettings.h"
#include "v8datamodel/PhysicsSettings.h"
#include "v8datamodel/CommonVerbs.h"
#include "script/LuaSettings.h"
#include "Util/Http.h"
#include "Util/Profiling.h"
#include "Util/StandardOut.h"
#include "script/LuaSettings.h"
#include "script/ScriptContext.h"
#include "Network/API.h"
#include "Network/Players.h"
#include "wbx/test/test_tools.h"
#include "gui/ProfanityFilter.h"

// Use this fixture to create a DataModel
class DataModelFixture
{
	static void init()
	{
		static boost::shared_ptr<wbx::ProfanityFilter> s_profanityFilter;
		s_profanityFilter = wbx::ProfanityFilter::getInstance();

		wbx::Profiling::init(false);

		static wbx::FactoryRegistrator registerFactoryObjects;

		// This is needed before you can instantiate a DataModel
		wbx::Http::init(wbx::Http::WinHttp, wbx::Http::CookieSharingSingleProcessMultipleThreads);

		wbx::GameSettings::singleton();
		wbx::LuaSettings::singleton();
		wbx::DebugSettings::singleton();
		wbx::PhysicsSettings::singleton();
	}
	boost::scoped_ptr<wbx::CommonVerbs> commonVerbs;
public:
	boost::shared_ptr<wbx::DataModel> dataModel;
	wbx::DataModel* operator&() { return dataModel.get(); }
	wbx::DataModel* operator->() { return dataModel.get(); }

	std::auto_ptr<wbx::Reflection::Tuple> execute(const char* script, const wbx::Reflection::Tuple& args, wbx::Security::Identities identity = wbx::Security::CmdLine_);
	std::auto_ptr<wbx::Reflection::Tuple> execute(const char* script)
	{
		wbx::Reflection::Tuple args;
		return execute(script, args);
	}

	void run()
	{
		wbx::RunService* runService = wbx::ServiceProvider::create<wbx::RunService>(dataModel.get());
		BOOST_REQUIRE(runService);
		runService->run();
	}

	DataModelFixture()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(&init, flag);
		dataModel = wbx::DataModel::createDataModel(true, new wbx::NullVerb(NULL,""), false);

		{
			wbx::DataModel::LegacyLock lock(dataModel.get(), wbx::DataModelJob::Write);
			commonVerbs.reset(new wbx::CommonVerbs(dataModel.get()));
		}
	}
	~DataModelFixture()
	{
		commonVerbs.reset();
		wbx::DataModel::closeDataModel(dataModel);
	}
};


class NetworkFixture
{
public:
	NetworkFixture()
	{
		static boost::once_flag flag = BOOST_ONCE_INIT;
		boost::call_once(&wbx::Network::initWithoutSecurity, flag);
	}

	static void startServer(DataModelFixture& dm)
	{
		const char* serverScript = "local ns = game:GetService('NetworkServer')\n"\
								   "ns:Start(53640, 0)\n";

		WBX_REQUIRE_NO_EXECEPTION(dm.execute(serverScript));
	}

	static void startClient(DataModelFixture& dm, bool waitForClientToReceiveCharacter=false);

private:
	static shared_ptr<wbx::Reflection::Tuple> accepted(wbx::CEvent* event)
	{
		//BOOST_TEST_MESSAGE("connection accepted");
		event->Set();
		return shared_ptr<wbx::Reflection::Tuple>();
	};

	static shared_ptr<wbx::Reflection::Tuple> failed(shared_ptr<const wbx::Reflection::Tuple> args, wbx::CEvent* event)
	{
		std::string reason = args->values[1].get<std::string>();
		BOOST_TEST_MESSAGE(reason);
		event->Set();
		return shared_ptr<wbx::Reflection::Tuple>();
	};

	static shared_ptr<wbx::Reflection::Tuple> rejected(shared_ptr<const wbx::Reflection::Tuple> args, wbx::CEvent* event)
	{
		BOOST_TEST_MESSAGE("connection rejected");
		event->Set();
		return shared_ptr<wbx::Reflection::Tuple>();
	};
};


// Use this fixture for debugging
class OutputLoggingFixture
{
	wbx::signals::scoped_connection messageConnection;
	static void print(const wbx::StandardOutMessage& message)
	{
		switch (message.type)
		{
		case wbx::MESSAGE_INFO:
			std::cout << "WBX INFO: " << message.message << '\n';
			break;
		case wbx::MESSAGE_WARNING:
			std::cout << "WBX WARNING: " << message.message << '\n';
			break;
		case wbx::MESSAGE_ERROR:
			std::cout << "WBX ERROR: " << message.message << '\n';
			break;
		default:
			std::cout << "WBX: " << message.message << '\n';
			break;
		}
	}
public:
	OutputLoggingFixture()
	{
		messageConnection = wbx::StandardOut::singleton()->messageOut.connect(&OutputLoggingFixture::print);
	}
};
