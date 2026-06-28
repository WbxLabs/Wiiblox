#include <boost/test/unit_test.hpp>

#include "util/URL.h"

BOOST_AUTO_TEST_SUITE(URLParser)

BOOST_AUTO_TEST_CASE(Split)
{
    {
        const std::string url_string = "https://www.Wiiblox.com/path";
        wbx::Url url = wbx::Url::fromString(url_string);
        BOOST_CHECK(url.isValid());
        BOOST_CHECK_EQUAL(url.scheme(), "https");
        BOOST_CHECK_EQUAL(url.host(), "www.Wiiblox.com");
        BOOST_CHECK_EQUAL(url.path(), "/path");
        BOOST_CHECK(url.query().empty());
        BOOST_CHECK(url.fragment().empty());
        BOOST_CHECK_EQUAL(url.asString(), url_string);
    }
    
    {
        const std::string url_string = "https://www.Wiiblox.com/path?query";
        wbx::Url url = wbx::Url::fromString(url_string);
        BOOST_CHECK(url.isValid());
        BOOST_CHECK_EQUAL(url.scheme(), "https");
        BOOST_CHECK_EQUAL(url.host(), "www.Wiiblox.com");
        BOOST_CHECK_EQUAL(url.path(), "/path");
        BOOST_CHECK_EQUAL(url.query(), "query");
        BOOST_CHECK(url.fragment().empty());
        BOOST_CHECK_EQUAL(url.asString(), url_string);
    }
    
    {
        const std::string url_string = "https://www.Wiiblox.com/path#fragment";
        wbx::Url url = wbx::Url::fromString(url_string);
        BOOST_CHECK(url.isValid());
        BOOST_CHECK_EQUAL(url.scheme(), "https");
        BOOST_CHECK_EQUAL(url.host(), "www.Wiiblox.com");
        BOOST_CHECK_EQUAL(url.path(), "/path");
        BOOST_CHECK(url.query().empty());
        BOOST_CHECK_EQUAL(url.fragment(), "fragment");
        BOOST_CHECK_EQUAL(url.asString(), url_string);
    }

    {
        const std::string url_string = "http://www.lengthy-example.com/~w/ith%20spaces?query=str%20ing&second=par%26am#and?frag%20ment";
        wbx::Url url = wbx::Url::fromString(url_string);
        BOOST_CHECK(url.isValid());
        BOOST_CHECK_EQUAL(url.scheme(), "http");
        BOOST_CHECK_EQUAL(url.host(), "www.lengthy-example.com");
        BOOST_CHECK_EQUAL(url.path(), "/~w/ith%20spaces");
        BOOST_CHECK_EQUAL(url.query(), "query=str%20ing&second=par%26am");
        BOOST_CHECK_EQUAL(url.fragment(), "and?frag%20ment");
        BOOST_CHECK_EQUAL(url.asString(), url_string);
    }
    
    {
        const std::string url_string = "WBXasset://fonts/character3.WBXm";
        wbx::Url url = wbx::Url::fromString(url_string);
        BOOST_CHECK(url.isValid());
        BOOST_CHECK_EQUAL(url.scheme(), "WBXasset");
        BOOST_CHECK_EQUAL(url.host(), "fonts");
        BOOST_CHECK_EQUAL(url.path(), "/character3.WBXm");
        BOOST_CHECK(url.query().empty());
        BOOST_CHECK(url.fragment().empty());
        BOOST_CHECK_EQUAL(url.asString(), url_string);
    }

    {
        wbx::Url url = wbx::Url::fromComponents("ftp", "topsite");
        BOOST_CHECK(url.isValid());
        BOOST_CHECK_EQUAL(url.asString(), "ftp://topsite/");
    }
}

BOOST_AUTO_TEST_CASE(Normalization)
{
    const std::string url_string = "HTtps://www.Wiiblox.com/../a/../.c/.//Bb?Qu/../Ue&rr=Y#./../Fr?aq";
    
    wbx::Url url = wbx::Url::fromString(url_string);
    BOOST_CHECK(url.isValid());
    BOOST_CHECK_EQUAL(url.scheme(), "https");
    BOOST_CHECK_EQUAL(url.host(), "www.Wiiblox.com");
    BOOST_CHECK_EQUAL(url.path(), "/.c/Bb");
    BOOST_CHECK_EQUAL(url.query(), "Qu/../Ue&rr=Y");
    BOOST_CHECK_EQUAL(url.fragment(), "./../Fr?aq");

    BOOST_CHECK_EQUAL(url.asString(),
                      "https://www.Wiiblox.com/.c/Bb?Qu/../Ue&rr=Y#./../Fr?aq");
}

namespace {
    std::string normalizePath(const std::string& path)
    {
        wbx::Url url = wbx::Url::fromComponents("http", "hostname", path);
        BOOST_CHECK(url.isValid());
        return url.path();
    }
} // unnamed namespace

BOOST_AUTO_TEST_CASE(PathNormalization)
{
    BOOST_CHECK_EQUAL(normalizePath(".."), "/");
    BOOST_CHECK_EQUAL(normalizePath("/.."), "/");
    
    BOOST_CHECK_EQUAL(normalizePath("../"), "/");
    BOOST_CHECK_EQUAL(normalizePath("/../"), "/");
    
    BOOST_CHECK_EQUAL(normalizePath("a/.."), "/");
    BOOST_CHECK_EQUAL(normalizePath("/a/.."), "/");
    BOOST_CHECK_EQUAL(normalizePath("/a/../"), "/");
    
    BOOST_CHECK_EQUAL(normalizePath("///"), "/");
    BOOST_CHECK_EQUAL(normalizePath("////"), "/");
    BOOST_CHECK_EQUAL(normalizePath("///a"), "/a");
    BOOST_CHECK_EQUAL(normalizePath("a///"), "/a/");
    BOOST_CHECK_EQUAL(normalizePath("//a/"), "/a/");
    
    BOOST_CHECK_EQUAL(normalizePath("/./"), "/");
    BOOST_CHECK_EQUAL(normalizePath("/a//b/./c"), "/a/b/c");
    
    BOOST_CHECK_EQUAL(normalizePath("/a/b///../c/..//.././.."), "/");
    BOOST_CHECK_EQUAL(normalizePath("/../a/../b///../c/..//.././../d/../e/f/./"), "/e/f/");
    
    BOOST_CHECK_EQUAL(normalizePath("/..a/..b/../.././../e/.././../"), "/");
}

BOOST_AUTO_TEST_CASE(Invalid)
{
    {
        wbx::Url url = wbx::Url::fromString("");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK_EQUAL(url.asString(), "");
    }
    
    {
        wbx::Url url = wbx::Url::fromString("http://");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK_EQUAL(url.asString(), "http:///");
    }
    
    {
        wbx::Url url = wbx::Url::fromString("/");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK_EQUAL(url.asString(), "/");
    }
    
    {
        wbx::Url url = wbx::Url::fromString("://hostname");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK_EQUAL(url.asString(), "hostname/");
    }

    {
        wbx::Url url = wbx::Url::fromComponents("", "", "");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK_EQUAL(url.asString(), "");
    }
    
    {
        wbx::Url url = wbx::Url::fromString("Wiiblox.com");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK(url.scheme().empty());
        BOOST_CHECK_EQUAL(url.host(), "Wiiblox.com");
        BOOST_CHECK_EQUAL(url.path(), "/");
        BOOST_CHECK(url.query().empty());
        BOOST_CHECK(url.fragment().empty());
        BOOST_CHECK_EQUAL(url.asString(), "Wiiblox.com/");
    }
    
    {
        wbx::Url url = wbx::Url::fromString("Wiiblox.com/path?#?");
        BOOST_CHECK(!url.isValid());
        BOOST_CHECK(url.scheme().empty());
        BOOST_CHECK_EQUAL(url.host(), "Wiiblox.com");
        BOOST_CHECK_EQUAL(url.path(), "/path");
        BOOST_CHECK(url.query().empty());
        BOOST_CHECK_EQUAL(url.fragment(), "?");
        BOOST_CHECK_EQUAL(url.asString(), "Wiiblox.com/path#?");
    }
}

BOOST_AUTO_TEST_CASE(Queries)
{
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com").isSubdomainOf("com"));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com").isSubdomainOf("Wiiblox.com"));
    BOOST_CHECK(wbx::Url::fromString("www.Wiiblox.com").isSubdomainOf("Wiiblox.com"));
    BOOST_CHECK(!wbx::Url::fromString("notWiiblox.com").isSubdomainOf("Wiiblox.com"));
    
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com").pathEquals(""));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com").pathEquals("/"));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com///").pathEquals("/"));
    BOOST_CHECK(!wbx::Url::fromString("Wiiblox.com/PATH").pathEquals("path"));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com/PATH").pathEqualsCaseInsensitive("path"));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com/long/path").pathEquals("long/path"));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com/long/path").pathEquals("/long/path"));
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com/long/path?query").pathEquals("long/path"));
    BOOST_CHECK(!wbx::Url::fromString("Wiiblox.com/long/path").pathEquals("long/path/with/stuff"));
    BOOST_CHECK(!wbx::Url::fromString("Wiiblox.com/long/path/with/stuff_here").pathEquals("long/path"));
    
    BOOST_CHECK(wbx::Url::fromString("Wiiblox.com/LONG/path").pathEqualsCaseInsensitive("long/PATH"));
}

BOOST_AUTO_TEST_SUITE_END()