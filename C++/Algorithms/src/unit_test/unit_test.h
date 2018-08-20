#pragma once

#include<iostream>
#include<string>
#include<sstream>
#include<exception>

#include<vector>
#include<algorithm>

namespace unit_test
{
	using namespace std;

	// Ostream operator for pair<Key, Value>
	template<class U, class V>
	ostream & operator<<(ostream & os, const pair<U, V> & p)
	{
		os << p.first << " " << p.second << endl;
		return os;
	}

	// Ostream operator for vector<T>
	template<class T>
	ostream & operator<<(ostream & os, const vector<T> & v)
	{
		std::copy(begin(v), end(v), ostream_iterator<T>(os, " "));
		return os;
	}

	// Comparison operator for vector (Equal) : nesessary for unit_test
	template<class T>
	bool operator==(const vector<T> & left, const vector<T> & right)
	{
		return std::equal(begin(left), end(left), begin(right));
	}

	// Comparison operator for vector (Not Equal) : nesessary for unit_test
	template<class T>
	bool operator!=(const vector<T> & left, const vector<T> & right)
	{
		return !(left == right);
	}



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