
#include <boost/test/unit_test.hpp>
#include <boost/test/framework.hpp>
#include "wbx/Debug.h"
#include "wbx/MathUtil.h"
#include "util/Statistics.h"
#include "GetTime.h"

#include "V8DataModel/ContentProvider.h"

#include "wbx/test/App.UnitTest.Lib.h"

using namespace wbx::Test;

BOOST_GLOBAL_FIXTURE(BaseGlobalFixture);
BOOST_GLOBAL_FIXTURE(AppGlobalFixture);

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
	unsigned statsRuns = 0;

	char** filteredArgv = new char*[argc];
	int filteredArgc = 0;

	for (int i = 0; i < argc; i++){
		unsigned runs = 0;
		if(!sscanf(argv[i], "--run_stats=%u", &runs))
		{
			if (!wbx::Test::BaseGlobalFixture::processArg(argv[i]))
				if (!wbx::Test::AppGlobalFixture::processArg(argv[i]))
					filteredArgv[filteredArgc++] = argv[i];
		}
		else
		{
			statsRuns = runs;
		}
	}	

	// prototype for user's unit test init function
#ifdef BOOST_TEST_ALTERNATIVE_INIT_API
	extern bool init_unit_test();

	boost::unit_test::init_unit_test_func init_func = &init_unit_test;
#else
	extern ::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] );

	boost::unit_test::init_unit_test_func init_func = &init_unit_test_suite;
#endif
	RakNet::Time start_time = RakNet::GetTime();
	int ret = ::boost::unit_test::unit_test_main( init_func, filteredArgc, filteredArgv );


	wbx::WindowAverage<double, double> runAverage(statsRuns);
	for(unsigned iRuns = 0; iRuns < statsRuns; iRuns++)
	{
		RakNet::Time start_test_time = RakNet::GetTime();
		::boost::unit_test::framework::run();
		runAverage.sample(RakNet::GetTime() - start_test_time);
	}

	delete [] filteredArgv;
	printf("------------------ Elapsed Time : %ums --------------------\n", (unsigned int)(RakNet::GetTime() - start_time));

	if(statsRuns > 0)
	{
		wbx::WindowAverage<double, double>::Stats runStats = runAverage.getSanitizedStats();
		double confMin, confMax;
		GetConfidenceInterval(runStats.average, runStats.variance, wbx::C90, &confMin, &confMax);

		printf("Average: %.2f ms, std: %.2f ms, Confidence: %.2f-%.2f ms\n", 
			runStats.average, sqrt(runStats.variance), confMin, confMax);
	}
	return ret;
}

extern bool init_unit_test()
{
	boost::unit_test::framework::master_test_suite().p_name.value = "App";	
	return true;
}

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
	init_unit_test();
	return 0;
}
