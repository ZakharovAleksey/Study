#ifndef HEADER_WEEK_2_H
#define HEADER_WEEK_2_H

#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;


#pragma region Декомпозиция программы

/*
	В курсе «Белый пояс по С++» у нас была задача «Автобусные остановки — 1». 
	В этой задаче вам будет дано её правильное решение, целиком содержащееся внутри функции main. 
	Вам надо будет выполнить декомпозицию этого решения на заранее заданные блоки так, 
	чтобы получившаяся программа так же корректно решала задачу. 
	Условие задачи «Автобусные остановки — 1» приведено ниже.
*/

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


#endif // !HEADER_WEEK_2_H
