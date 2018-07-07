#include "date.h"
#include"test_runner.h"

#include <iomanip>

ostream & operator<<(ostream & os, const Date & date)
{
	os  << setw(4) << setfill('0') << date.year  << "-"
		<< setw(2) << setfill('0') << date.month << "-"
		<< setw(2) << setfill('0') << date.day;

	return os;
}

bool Date::operator<(const Date & other) const
{
	return tie(year, month, day) < tie(other.year, other.month, other.day);
}

bool Date::operator==(const Date & other) const
{
	return tie(year, month, day) == tie(other.year, other.month, other.day);
}

bool Date::operator>(const Date & other) const
{
	return !(*this < other) && !(*this == other);
}

bool Date::operator<=(const Date & other) const
{
	return (*this < other) || (*this == other);
}

bool Date::operator>=(const Date & other) const
{
	return (*this > other) || (*this == other);
}

bool Date::operator!=(const Date & other) const
{
	return !(*this == other);
}

Date ParseDate(istringstream & is)
{
	Date date;
	string tmp;
	getline(is, tmp, '-');
	date.year = stoi(tmp);
	getline(is, tmp, '-');
	date.month = stoi(tmp);
	getline(is, tmp, ' ');
	date.day = stoi(tmp);

	return date;
}

void TestDateComparison()
{
	// Less
	Assert(Date{ 2017, 1, 1 } < Date{ 2018, 7, 8 }, "Date less test 1");
	Assert(Date{ 2017, 1, 1 } < Date{ 2017, 7, 1 }, "Date less test 2");
	Assert(Date{ 2017, 1, 1 } < Date{ 2017, 1, 8 }, "Date less test 3");
	// Les or equal
	Assert(Date{ 2017, 1, 1 } <= Date{ 2018, 7, 8 }, "Date less or equal test 1");
	Assert(Date{ 2017, 1, 1 } <= Date{ 2017, 7, 1 }, "Date less or equal test 2");
	Assert(Date{ 2017, 1, 1 } <= Date{ 2017, 1, 8 }, "Date less or equal test 3");
	Assert(Date{ 2017, 1, 1 } <= Date{ 2017, 1, 8 }, "Date less or equal test 4");
	// Greater
	Assert(Date{ 2018, 7, 8 } > Date{ 2017, 1, 1 }, "Date greater test 1");
	Assert(Date{ 2017, 7, 1 } > Date{ 2017, 1, 1 }, "Date greater test 2");
	Assert(Date{ 2017, 1, 8 } > Date{ 2017, 1, 1 }, "Date greater test 3");
	// Greater or equal
	Assert(Date{ 2018, 7, 8 } >= Date{ 2017, 1, 1 }, "Date greater or equal test 1");
	Assert(Date{ 2017, 7, 1 } >= Date{ 2017, 1, 1 }, "Date greater or equal test 2");
	Assert(Date{ 2017, 1, 8 } >= Date{ 2017, 1, 1 }, "Date greater or equal test 3");
	Assert(Date{ 2017, 1, 1 } >= Date{ 2017, 1, 1 }, "Date greater or equal test 4");
	// Equal
	Assert(Date{ 2017, 1, 1 } == Date{ 2017, 1, 1 }, "Date equal");
	// Not Equal
	Assert(Date{ 2018, 1, 1 } != Date{ 2017, 1, 1 }, "Date not equal test 1");
	Assert(Date{ 2017, 2, 1 } != Date{ 2017, 1, 1 }, "Date not equal test 2");
	Assert(Date{ 2017, 1, 2 } != Date{ 2017, 1, 1 }, "Date not equal test 3");
}

void TestDateOutput()
{
	ostringstream os;
	os << Date{ 2017, 12, 11 };
	AssertEqual(os.str(), "2017-12-11", "Date full");
	os = ostringstream();

	os << Date{ 2017, 12, 1 };
	AssertEqual(os.str(), "2017-12-01", "Date not full day");
	os = ostringstream();

	os << Date{ 2017, 1, 1 };
	AssertEqual(os.str(), "2017-01-01", "Date not full day and month");
	os = ostringstream();

	os << Date{ 93, 1, 1 };
	AssertEqual(os.str(), "0093-01-01", "Date not full day, month, year");
	os = ostringstream();
}

