/*
 *  HTTP.cpp
 *  MacClient
 *
 *  Created by Rick Kimball on 12/7/10.
 *  Copyright 2010 Wiiblox Inc. All rights reserved.
 *
 */
#include "FastLog.h"

#include <boost/test/unit_test.hpp>
#include <boost/scoped_ptr.hpp>
#include "Util/Http.h"
#include "Util/Guid.h"
#include <sstream>
#include "wbx/test/ScopedFastFlagSetting.h"
#include "wbx/test/test_tools.h"
#include "wbx/test/DataModelFixture.h"

#include "Server.h"
#include "V8Xml/WebParser.h"
#include "v8datamodel/HttpWbxApiService.h"

BOOST_AUTO_TEST_SUITE(HTTPSuite)

static void testGet(wbx::Http& http, bool externalRequest = false)
{
	std::string response1a;
	http.get(response1a, externalRequest);

    if (externalRequest)
    {
        BOOST_CHECK_EQUAL(std::string::npos != response1a.find("\"url\": \"http://httpbin.org/get\""), true);
    }
    else
    {
        BOOST_CHECK_EQUAL(response1a.substr(0, 6),"<html>");
    }
}

static void testGetFailure(wbx::Http& http, bool externalRequest)
{
	std::string response1a;
	BOOST_CHECK_THROW(http.get(response1a, externalRequest), std::exception);
}

static void testPost(wbx::Http& http)
{
    std::stringstream formdata;
    formdata << "test=fame";

    std::string response;
    http.post(formdata, wbx::Http::kContentTypeUrlEncoded, false, response, true);
    BOOST_CHECK_EQUAL(std::string::npos != response.find("\"test\": \"fame\""), true);
}

std::string guidify(std::string url)
{
	if (url.find('?') == std::string::npos)
		return url + "?" + wbx::Guid().readableString(12);
	else
		return url + "&" + wbx::Guid().readableString(12);
}

static void PostAsyncSuccess(std::string response)
{
	BOOST_CHECK_EQUAL(response, "\"RGS OK\"");
}
static void PostAsyncError(std::string error)
{
	BOOST_CHECK_EQUAL(true, false);
}

static void GetAsyncSuccess(std::string response)
{
	shared_ptr<const wbx::Reflection::ValueTable> table(wbx::make_shared<const wbx::Reflection::ValueTable>());
	wbx::WebParser::parseJSONTable(response, table);

	wbx::Reflection::Variant assetVariant = table->at("AssetId");
	if (assetVariant.isType<int>())
	{
		int asset = assetVariant.cast<int>();
		BOOST_CHECK_EQUAL(asset, 28277486);
	}

	wbx::Reflection::Variant productVariant = table->at("ProductId");
	if (productVariant.isType<int>())
	{
		int product = productVariant.cast<int>();
		BOOST_CHECK_EQUAL(product, 4678951);
	}
}
static void GetAsyncError(std::string error)
{
	BOOST_CHECK_EQUAL(true, false);
}

BOOST_AUTO_TEST_CASE(HTTP_SYNC_GET_TESTS)
{
	{
		// Make a synchronous get request expecting data
		{
			wbx::Http a(guidify("http://www.Wiiblox.com/Info/EULA.htm"));
			WBX_TEST_WITH_TIMEOUT(boost::bind(&testGet, boost::ref(a), false), wbx::Time::Interval(10));
			wbx::Http b(guidify("http://www.Wiiblox.com/Info/EULA.htm"), wbx::Http::WinHttp);
			WBX_TEST_WITH_TIMEOUT(boost::bind(&testGet, boost::ref(b), false), wbx::Time::Interval(10));
			wbx::Http c(guidify("http://www.Wiiblox.com/Info/EULA.htm"), wbx::Http::WinInet);
			WBX_TEST_WITH_TIMEOUT(boost::bind(&testGet, boost::ref(c), false), wbx::Time::Interval(10));
		}
	
		{
			wbx::Http a(guidify("http://blahblah.Wiiblox.com/this_should_fail.php"));
			WBX_TEST_WITH_TIMEOUT(boost::bind(&testGetFailure, boost::ref(a), false), wbx::Time::Interval(10));
			wbx::Http b(guidify("http://blahblah.Wiiblox.com/this_should_fail.php"), wbx::Http::WinHttp);
			WBX_TEST_WITH_TIMEOUT(boost::bind(&testGetFailure, boost::ref(b), false), wbx::Time::Interval(10));
			wbx::Http c(guidify("http://blahblah.Wiiblox.com/this_should_fail.php"), wbx::Http::WinInet);
			WBX_TEST_WITH_TIMEOUT(boost::bind(&testGetFailure, boost::ref(c), false), wbx::Time::Interval(10));
		}
		
		// Need test for iostream read
		
	}
}

BOOST_AUTO_TEST_CASE(HTTP_SYNC_GET_EXTERNAL_TESTS)
{
    {
        wbx::Http a("http://httpbin.org/get");
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testGet, boost::ref(a), true), wbx::Time::Interval(10));
        wbx::Http b("http://httpbin.org/get", wbx::Http::WinHttp);
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testGet, boost::ref(b), true), wbx::Time::Interval(10));
        wbx::Http c("http://httpbin.org/get", wbx::Http::WinInet);
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testGet, boost::ref(c), true), wbx::Time::Interval(10));
    }

    {
        wbx::Http a("http://httpbin.org/status/404");
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testGetFailure, boost::ref(a), true), wbx::Time::Interval(10));
        wbx::Http b("http://httpbin.org/status/404", wbx::Http::WinHttp);
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testGetFailure, boost::ref(b), true), wbx::Time::Interval(10));
        wbx::Http c("http://httpbin.org/status/404", wbx::Http::WinInet);
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testGetFailure, boost::ref(c), true), wbx::Time::Interval(10));
    }
}

BOOST_AUTO_TEST_CASE(HTTP_SYNC_POST_EXTERNAL_TESTS)
{
    {
        std::string response;
        wbx::Http a("http://httpbin.org/post");
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testPost, boost::ref(a)), wbx::Time::Interval(10));
        wbx::Http b("http://httpbin.org/post", wbx::Http::WinHttp);
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testPost, boost::ref(b)), wbx::Time::Interval(10));
        wbx::Http c("http://httpbin.org/post", wbx::Http::WinInet);
        WBX_TEST_WITH_TIMEOUT(boost::bind(&testPost, boost::ref(c)), wbx::Time::Interval(10));
    }
}


static void doHTTP_SYNC_POST_TESTS()
{
	std::stringstream formdata;

	formdata << "";

	std::string url = "https://games.api.sitetest1.Wiibloxlabs.com/";
	wbx::Http postrequest1a(url);
	std::string response1a;
	postrequest1a.post(formdata, wbx::Http::kContentTypeApplicationJson, false, response1a);
	BOOST_CHECK_EQUAL(response1a, "\"RGS OK\"");

	// Need test for iostream write/read
}

BOOST_AUTO_TEST_CASE(HTTP_SYNC_POST_TESTS)
{
	WBX_TEST_WITH_TIMEOUT(doHTTP_SYNC_POST_TESTS, wbx::Time::Interval(10));
}

BOOST_AUTO_TEST_CASE(HTTP_URL_ENCODE_TEST)
{
	BOOST_CHECK_EQUAL(wbx::Http::urlEncode("this is a&#test"), "this%20is%20a%26%23test");
}

BOOST_AUTO_TEST_CASE(HTTP_URL_DECODE_TEST)
{
	std::string allChars;
	for (int i = 1; i < 256; ++i)
	{
		allChars += (char)i;
	}
	BOOST_REQUIRE_EQUAL(allChars.size(), 255);
	BOOST_CHECK_EQUAL(allChars, wbx::Http::urlDecode(wbx::Http::urlEncode(allChars)));
}

BOOST_AUTO_TEST_CASE(API_PROXY_HTTP_SYNC_GET_TESTS)
{
	DataModelFixture dm;

	wbx::DataModel::LegacyLock l(&dm, wbx::DataModelJob::Write);

	wbx::Reflection::Tuple args;
	dm.execute("game:GetService('ContentProvider'):SetBaseUrl('http://www.Wiiblox.com/')", args);

	NetworkFixture::startServer(dm);
	wbx::HttpWbxApiService* apiService = wbx::ServiceProvider::create<wbx::HttpWbxApiService>(&dm);

	std::string responseString;
	apiService->get("marketplace/productinfo?assetId=28277486", true, wbx::PRIORITY_EXTREME, responseString);

	shared_ptr<const wbx::Reflection::ValueTable> table(wbx::make_shared<const wbx::Reflection::ValueTable>());
	wbx::WebParser::parseJSONTable(responseString, table);

	wbx::Reflection::Variant assetVariant = table->at("AssetId");
	if (assetVariant.isType<int>())
	{
		int asset = assetVariant.cast<int>();
		BOOST_CHECK_EQUAL(asset, 28277486);
	}

	wbx::Reflection::Variant productVariant = table->at("ProductId");
	if (productVariant.isType<int>())
	{
		int product = productVariant.cast<int>();
		BOOST_CHECK_EQUAL(product, 4678951);
	}
}

BOOST_AUTO_TEST_CASE(API_PROXY_HTTP_ASYNC_GET_TESTS)
{
	DataModelFixture dm;

	wbx::DataModel::LegacyLock l(&dm, wbx::DataModelJob::Write);

	wbx::Reflection::Tuple args;
	dm.execute("game:GetService('ContentProvider'):SetBaseUrl('http://www.Wiiblox.com/')", args);

	NetworkFixture::startServer(dm);
	wbx::HttpWbxApiService* apiService = wbx::ServiceProvider::create<wbx::HttpWbxApiService>(&dm);

	apiService->getAsync("marketplace/productinfo?assetId=28277486", true, wbx::PRIORITY_EXTREME,
		boost::bind(&GetAsyncSuccess, _1),
		boost::bind(&GetAsyncError, _1) );

	G3D::System::sleep(3.0);
}

BOOST_AUTO_TEST_CASE(API_PROXY_HTTP_ASYNC_POST_TESTS)
{
	DataModelFixture dm;

	wbx::DataModel::LegacyLock l(&dm, wbx::DataModelJob::Write);

	wbx::Reflection::Tuple args;
	dm.execute("game:GetService('ContentProvider'):SetBaseUrl('http://games.www.sitetest1.Wiibloxlabs.com/')", args);

	wbx::HttpWbxApiService* apiService = wbx::ServiceProvider::create<wbx::HttpWbxApiService>(&dm);
	wbx::ContentProvider* cp = wbx::ServiceProvider::create<wbx::ContentProvider>(&dm);

	NetworkFixture::startServer(dm);

	std::string apiBaseUrl = cp->getApiBaseUrl();
    wbx::Http http(apiBaseUrl);
    std::string postString(" ");
	apiService->postAsync(http, postString, wbx::PRIORITY_EXTREME, wbx::HttpService::APPLICATION_JSON,
		boost::bind(&PostAsyncSuccess, _1),
		boost::bind(&PostAsyncError, _1));

	G3D::System::sleep(3.0);
}


BOOST_AUTO_TEST_SUITE_END()

