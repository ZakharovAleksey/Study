#include"unit_test.h"

namespace unit_test
{
	TestRunner::TestRunner() : failed_count_(0) {}

	TestRunner::~TestRunner()
	{
		if (failed_count_ != 0)
		{
			cerr << "Not all tests has been passed successfully. Number of failed tests N = " << failed_count_ << ". Terminate." << endl;
			exit(1);
		}
		cerr << "All tests has been passed succesfully." << endl;
	}
}