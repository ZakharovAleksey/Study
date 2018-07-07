#pragma once

#include<string>
#include<sstream>
#include<tuple>

using namespace std;

struct Date
{
	int year;
	int month;
	int day;

	friend ostream & operator<<(ostream & os, const Date & date);

	// Nesesary for usage Date as a key for map datastructure
	bool operator<(const Date& other) const;
	// Nesessay for Node Evaluate function
	bool operator==(const Date& other) const;
	bool operator>(const Date& other) const;
	bool operator<=(const Date& other) const;
	bool operator>=(const Date& other) const;
	bool operator!=(const Date& other) const;
};

Date ParseDate(istringstream & is);

// Tests
void TestDateComparison();
void TestDateOutput();
