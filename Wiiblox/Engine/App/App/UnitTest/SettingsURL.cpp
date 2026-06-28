#include <boost/test/unit_test.hpp>

#include "WiibloxServicesTools.h"

BOOST_AUTO_TEST_SUITE(SettingsUrl)

BOOST_AUTO_TEST_CASE(SettingsUrlProd)
{
	std::string testUrl = "http://clientsettings.api.Wiiblox.com/Setting/QuietGet/Test/?apiKey=TestKey";
	std::string url = GetSettingsUrl("www.Wiiblox.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);

	url = GetSettingsUrl("http://www.Wiiblox.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);
}

BOOST_AUTO_TEST_CASE(SettingsUrlNewGametest)
{
	std::string testUrl = "http://clientsettings.api.gametest1.Wiibloxlabs.com/Setting/QuietGet/Test/?apiKey=TestKey";
	std::string url = GetSettingsUrl("www.gametest1.Wiibloxlabs.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);

	url = GetSettingsUrl("http://www.gametest1.Wiibloxlabs.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);
}

BOOST_AUTO_TEST_CASE(SettingsUrlGametest)
{
	std::string testUrl = "http://clientsettings.api.gametest1.Wiiblox.com/Setting/QuietGet/Test/?apiKey=TestKey";
	std::string url = GetSettingsUrl("www.gametest1.Wiiblox.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);

	url = GetSettingsUrl("http://www.gametest1.Wiiblox.com", "Test", "TestKey");
	BOOST_CHECK_EQUAL(url, testUrl);
}

BOOST_AUTO_TEST_SUITE_END()
