
#include "wbx/test/App.UnitTest.Lib.h"

#include <boost/test/unit_test.hpp>

#include "V8DataModel/ContentProvider.h"
#include "util/Statistics.h"
#include "wbx/Debug.h"

#include "boost/filesystem.hpp"
namespace fs = boost::filesystem;

namespace WBX {
	extern bool nameThreads;

}

wbx::ProtectedString wbx::ProtectedString::fromTestSource(const std::string& stringRef)
{
    return wbx::ProtectedString::fromTrustedSource(stringRef);
}

using namespace wbx::Test;

static bool setBaseUrl = false;
static bool setContentFolder = false;

AppGlobalFixture::AppGlobalFixture()
{
	wbx::nameThreads = false;

	if (!setBaseUrl)
		// TODO: Is this right?
		::SetBaseURL("www.Wiiblox.com");

	if (!setContentFolder)
	{
		// TODO: Make this configurable?
		fs::path path = fs::initial_path<fs::path>();
#ifdef _WIN32
		// On Windows App.UnitTest is run from inside of Client/App.UnitTest/bin/Release folder
		// working path above is pointing to Client
		// Content folder is expected directory underneath the Client
        path = path.parent_path();
		path /= "Content";
#else
		// On Mac App.UnitTest is run from inside of Client/build/Release folder
		// working path above is pointing to Client/build
		// Content folder is expected directory underneath the Client
		path /= "content";
#endif
		std::string filePath = path.string();
		wbx::ContentProvider::setAssetFolder(filePath.c_str());
	}
}

bool AppGlobalFixture::processArg( const std::string arg )
{
	if (arg.find("--help") == 0)
	{
		std::cout << "Wiiblox custom arguments:\n";
		std::cout << "   --content_path=<dir> - directory to the Content folder. Can be absolute or relative\n";
		std::cout << "   --base_url=<url> - define the BaseUrl\n";
		return false;
	}
	const char* contentFolderArg = "--content_path=";
	if (arg.find(contentFolderArg) == 0)
	{
		std::string contentFolderValue = arg.substr(strlen(contentFolderArg));
		fs::path contentPath = contentFolderValue;
		if (!contentPath.has_root_path())
			contentPath = fs::initial_path<fs::path>() / contentFolderValue;
		wbx::ContentProvider::setAssetFolder(contentPath.string().c_str());
		setContentFolder = true;
		return true;
	}

	const char* baseUrlArg = "--base_url=";
	if (arg.find(baseUrlArg) == 0)
	{
		std::string baseUrlValue = arg.substr(strlen(baseUrlArg));
		::SetBaseURL(baseUrlValue.c_str());
		setBaseUrl = true;
		return true;
	}

	return false;

}


