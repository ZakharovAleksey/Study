#pragma once

#include"date.h"
#include"test_runner.h"

#include<map>
#include<set>
#include<algorithm>
#include <functional>
#include<iterator>


using namespace std;

using DataBaseBody = map<Date, set<string>>;
using DataBaseOrder = multimap<Date, set<string>::iterator>;

template<class Key, class Value>
ostream & operator<<(ostream & os, pair<Key, Value> p)
{
	os << p.first << " " << p.second;
	return os;
}

class Database
{
public:
	Database();

	void Add(const Date & date, const string & event);
	ostream & Print(ostream & os);

	using Func = function<bool(const Date& Date, const string& event)>;
	
	size_t RemoveIf(Func predicate);
	const multimap<Date, string> FindIf(Func predicate) const;

	const string Last(const Date & date);

private:
	DataBaseBody body_;
	DataBaseOrder order_;
};