#ifndef UNIT_TEST_H
#define UNIT_TEST_H

#pragma once

#include <iostream>

#include <sstream>
#include <exception>
#include <string>

namespace unit_test
{
	using namespace std;

	template<class T, class U>
	void AssertEqual(const T & left, const U & right, const string hint)
	{
		if (right != left)
		{
			ostringstream os;
			os << left << " != " << right << " Hint: " << hint;
			throw runtime_error(os.str());
		}
	}

	class TestRunner
	{
	public:

		TestRunner();
		~TestRunner();

		template<class TestFunc>
		void RunTest(TestFunc test, const string & test_name)
		{
			try
			{
				test();
				cerr << test_name << " OK" << endl;
			}
			catch (runtime_error & e)
			{
				++fail_count_;
				cerr << test_name << " failed. Error: " << e.what() << endl;
			}
		}

	private:
		size_t fail_count_;
	};
}

#endif // !UNIT_TEST_H
