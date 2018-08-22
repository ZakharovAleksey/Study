#ifndef HEADER_WEEK_2_H
#define HEADER_WEEK_2_H

#pragma once

#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

#include <sstream>
#include <exception>
#include <string>

using namespace std;

#pragma region Декомпозиция программы

enum class QueryType {
	NewBus,
	BusesForStop,
	StopsForBus,
	AllBuses
};

struct Query {
	QueryType type;
	string bus;
	string stop;
	vector<string> stops;
};

istream& operator >> (istream& is, Query& q)
{
	string type; is >> type;

	if (type == "NEW_BUS")
	{
		q.type = QueryType::NewBus;

		is >> q.bus;
		int stop_count;
		is >> stop_count;

		q.stops.resize(stop_count);
		for (string& stop : q.stops)
			is >> stop;
	}
	else if (type == "BUSES_FOR_STOP")
	{
		q.type = QueryType::BusesForStop;
		is >> q.stop;
	}
	else if (type == "STOPS_FOR_BUS")
	{
		q.type = QueryType::StopsForBus;
		is >> q.bus;
	}
	else if (type == "ALL_BUSES")
	{
		q.type = QueryType::AllBuses;
	}

	return is;
}


struct BusesForStopResponse
{
	vector<string> stops_to_buses;
	BusesForStopResponse(const vector<string> & stops_to_buses) : stops_to_buses(stops_to_buses) {}
};

ostream& operator << (ostream& os, const BusesForStopResponse& r)
{
	if (r.stops_to_buses.empty())
		os << "No stop" << endl;
	else
	{
		for (const string& bus : r.stops_to_buses) {
			os << bus << " ";
		}
		os << endl;
	}

	return os;
}


struct StopsForBusResponse
{
	map<string, vector<string>> buses_to_stops;
	map<string, vector<string>> stops_to_buses;
	string bus;

	StopsForBusResponse() {}
	StopsForBusResponse(const map<string, vector<string>> & buses_to_stops,
		const map<string, vector<string>> & stops_to_buses,
		const string & bus) :
		buses_to_stops(buses_to_stops), stops_to_buses(stops_to_buses), bus(bus) {}
};

ostream& operator << (ostream& os, const StopsForBusResponse& r)
{
	if (r.buses_to_stops.count(r.bus) == 0) {
		os << "No bus" << endl;
	}
	else {
		for (const string& stop : r.buses_to_stops.at(r.bus)) {
			os << "Stop " << stop << ": ";
			if (r.stops_to_buses.at(stop).size() == 1) {
				os << "no interchange";
			}
			else {
				for (const string& other_bus : r.stops_to_buses.at(stop)) {
					if (r.bus != other_bus) {
						os << other_bus << " ";
					}
				}
			}
			os << endl;
		}
	}
	return os;
}


struct AllBusesResponse {
	map<string, vector<string>> buses_to_stops;

	AllBusesResponse() {}
	AllBusesResponse(const map<string, vector<string>> & buses_to_stops) :
		buses_to_stops(buses_to_stops) {}
};

ostream& operator << (ostream& os, const AllBusesResponse& r)
{
	if (r.buses_to_stops.empty()) {
		os << "No buses" << endl;
	}
	else {
		for (const auto& bus_item : r.buses_to_stops) {
			os << "Bus " << bus_item.first << ": ";
			for (const string& stop : bus_item.second) {
				os << stop << " ";
			}
			os << endl;
		}
	}
	return os;
}


class BusManager {
public:
	void AddBus(const string& bus, const vector<string>& stops)
	{

		buses_to_stops.insert(make_pair(bus, stops));

		for (const string& stop : stops)
			stops_to_buses[stop].push_back(bus);
	}

	BusesForStopResponse GetBusesForStop(const string& stop) const
	{
		if (stops_to_buses.count(stop) == 0)
			return BusesForStopResponse(vector<string>());
		else
			return BusesForStopResponse(stops_to_buses.at(stop));
	}

	StopsForBusResponse GetStopsForBus(const string& bus) const
	{
		return StopsForBusResponse(buses_to_stops, stops_to_buses, bus);
	}

	AllBusesResponse GetAllBuses() const
	{
		return AllBusesResponse(buses_to_stops);
	}

private:

	map<string, vector<string>> buses_to_stops;
	map<string, vector<string>> stops_to_buses;
};

#pragma endregion

template<class T, class U>
void AssertEqual(const T& t, const U& u, const string& hint = {}) {
	if (t != u) {
		ostringstream os;
		os << "Assertion failed: " << t << " != " << u;
		if (!hint.empty()) {
			os << " hint: " << hint;
		}
		throw runtime_error(os.str());
	}
}

void Assert(bool b, const string& hint) {
	AssertEqual(b, true, hint);
}

#pragma region Тесты для класса Person

class Person {
public:
	void ChangeFirstName(int year, const string& first_name) { }
	void ChangeLastName(int year, const string& last_name) { }
	string GetFullName(int year) { }
};

void TestPredefinedLastNameFirst() {
	Person person;

	person.ChangeLastName(1, "LN1");
	person.ChangeFirstName(3, "FN1");

	AssertEqual(person.GetFullName(0), "Incognito");
	AssertEqual(person.GetFullName(2), "LN1 with unknown first name");
	AssertEqual(person.GetFullName(4), "FN1 LN1");
}

void TestPredefined() {
	Person person;

	person.ChangeFirstName(1, "FN1");
	person.ChangeLastName(3, "LN1");

	AssertEqual(person.GetFullName(0), "Incognito");
	AssertEqual(person.GetFullName(2), "FN1 with unknown last name");
	AssertEqual(person.GetFullName(4), "FN1 LN1");

	person.ChangeFirstName(5, "FN2");
	AssertEqual(person.GetFullName(4), "FN1 LN1");
	AssertEqual(person.GetFullName(5), "FN2 LN1");
	AssertEqual(person.GetFullName(6), "FN2 LN1");

	person.ChangeLastName(4, "LN2");
	AssertEqual(person.GetFullName(3), "FN1 LN1");
	AssertEqual(person.GetFullName(4), "FN1 LN2");
	AssertEqual(person.GetFullName(5), "FN2 LN2");
}

#pragma endregion

#pragma region Тесты для класса Rational

class Rational {
public:
	Rational() {}
	Rational(int numerator, int denominator) { }

	int Numerator() const { }
	int Denominator() const { }
};

void TestRational(int num, int denom, int exp_num, int exp_denom)
{
	ostringstream os;
	os << num << " / " << denom << " -> " << exp_num << " / " << exp_denom << endl;

	AssertEqual(Rational(num, denom).Numerator(), exp_num,		os.str() + " numerator\n");
	AssertEqual(Rational(num, denom).Denominator(), exp_denom,  os.str() + " denominator\n");
}

void RationalTests()
{
	{
		// Constructor (condition 1)
		AssertEqual(Rational().Numerator(), 0,		"default constructor num"); 
		AssertEqual(Rational().Denominator(), 1,	"default constructor denom");
		// Condition 2 (From task)
		TestRational(2, 4, 1, 2);
		TestRational(-2, -4, 1, 2);
		TestRational(4, 2, 2, 1);
		TestRational(-4, -4, 2, 1);
		TestRational(20, 6, 10, 3);
		TestRational(-20, -6, 10, 3);
		// Condition 3 (From task)
		TestRational(-2, 3, -2, 3);
		TestRational(-3, 2, -3, 2);
		TestRational(2, -3, -2, 3);
		TestRational(3, -2, -3, 2);
		// Condition 4 (From task)
		TestRational(2, 3, 2, 3);
		TestRational(3, 2, 3, 2);
		TestRational(-2, -3, 2, 3);
		TestRational(-3, -2, 3, 2);
		// Condition 5 (From task)
		TestRational(0, 3, 0, 1);
		TestRational(0, -3, 0, 1);
	}
}

#pragma endregion

#endif // !HEADER_WEEK_2_H
