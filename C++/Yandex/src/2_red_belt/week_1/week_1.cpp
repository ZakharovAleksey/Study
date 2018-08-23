#include"week_1.h"
#include"../../unit_test/unit_test.h"

#include <algorithm>
#include <numeric>
#include<tuple>

namespace week_1
{
	bool operator==(const Date & left, const Date & right)
	{
		return tie(left.year, left.month, left.day) == tie(right.year, right.month, right.day);
	}

	bool operator!=(const Date & left, const Date & right)
	{
		return !(left == right);
	}

	bool operator<(const Date & left, const Date & right)
	{
		return tie(left.year, left.month, left.day) < tie(right.year, right.month, right.day);
	}

	ostream & operator<<(ostream & os, const Date & d)
	{
		os << d.year << "-" << d.month << "-" << d.day << endl;
		return os;
	}

	istream & operator >> (istream & is, Date & d)
	{
		is >> d.year; is.get();
		is >> d.month; is.get();
		is >> d.day;
		return is;
	}

	bool operator==(const Time & left, const Time & right)
	{
		return tie(left.hours, left.minutes) == tie(right.hours, right.minutes);
	}

	bool operator!=(const Time & left, const Time & right)
	{
		return !(left == right);
	}

	bool operator<(const Time & left, const Time & right)
	{
		return tie(left.hours, left.minutes) < tie(right.hours, right.minutes);
	}

	ostream & operator<<(ostream & os, const Time & d)
	{
		os << d.hours << ":" << d.minutes << endl;
		return os;
	}

	istream & operator >> (istream & is, Time & d)
	{
		is >> d.hours; is.get();
		is >> d.minutes;
		return is;
	}

	void TestDeque()
	{
		week_1::Deque<int> d;
		ASSERT_EQUAL(d.Empty(), true);
		d.PushFront(10);
		ASSERT_EQUAL(d.Front(), 10);
		ASSERT_EQUAL(d.Back(), 10);
		ASSERT_EQUAL(d.Size(), 1u);
		ASSERT_EQUAL(d.Empty(), false);
		d.PushFront(20);
		ASSERT_EQUAL(d.Front(), 20);
		ASSERT_EQUAL(d.Back(), 10);
		ASSERT_EQUAL(d.Size(), 2u);
		ASSERT_EQUAL(d.Empty(), false);
		d.PushBack(0);
		ASSERT_EQUAL(d.Front(), 20);
		ASSERT_EQUAL(d.Back(), 0);
		ASSERT_EQUAL(d.Size(), 3u);
		ASSERT_EQUAL(d.Empty(), false);
		d.PushBack(-10);
		ASSERT_EQUAL(d.Front(), 20);
		ASSERT_EQUAL(d.Back(), -10);
		ASSERT_EQUAL(d.Size(), 4u);
		ASSERT_EQUAL(d.Empty(), false);

		int & new_front = d.Front();
		new_front = 30;
		ASSERT_EQUAL(d.Front(), 30);
		int & new_back = d.Back();
		new_back = -20;
		ASSERT_EQUAL(d.Back(), -20);

		ASSERT_EQUAL(d[0], 30);
		ASSERT_EQUAL(d.At(0), 30);
		ASSERT_EQUAL(d[1], 10);
		ASSERT_EQUAL(d.At(1), 10);
		ASSERT_EQUAL(d[2], 0);
		ASSERT_EQUAL(d.At(2), 0);
		ASSERT_EQUAL(d[3], -20);
		ASSERT_EQUAL(d.At(3), -20);

		try
		{
			ASSERT_EQUAL(d.At(5), 30);
		}
		catch (const out_of_range & e)
		{
			string exp = "Index is bigger than number of elements.";
			ASSERT_EQUAL(exp, e.what());
		}
	}

}