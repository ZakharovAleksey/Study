#pragma once

#include<iostream>
#include<string>
#include<sstream>
#include<exception>

namespace unit_test
{
	using namespace std;

	template<class U, class V>
	void AssertEqual(const U & left, const V right, const string & hint)
	{
		if (left != right)
		{
			ostringstream os;
			os << left << " != " << right << " Hint : " << hint;
			throw runtime_error(os.str());
		}
	}

	class TestRunner
	{
	public:
		TestRunner();
		~TestRunner();

		template<class Function>
		void RunTest(Function test, const string & test_name)
		{
			try
			{
				test();
				cerr << test_name << " is OK" << endl;
			}
			catch (const runtime_error & e)
			{
				++failed_count_;
				cerr << "Test " << test_name << " is failed. Hint : " << e.what() << endl;
			}
		}

	private:
		size_t failed_count_;
	};


}