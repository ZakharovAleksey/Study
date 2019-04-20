#include"week_5.h"

#include<algorithm>
#include<iterator>
#include<sstream>

namespace WhiteBeltW5 {

	bool WhiteBeltW5::operator<(const MYear & left, const MYear & right){
		return left.d_year < right.d_year;
	}

	bool operator==(const MYear & left, const MYear & right)
	{
		return left.d_year == right.d_year;
	}

	ostream & operator<<(ostream & os, const MYear & year) {
		os << year.d_year;
		return os;
	}

	bool operator<(const MMonth & left, const MMonth & right) {
		return left.d_month < right.d_month;
	}

	bool operator==(const MMonth & left, const MMonth & right) {
		return left.d_month == right.d_month;
	}

	ostream & operator<<(ostream & os, const MMonth & month) {
		os << month.d_month;
		return os;
	}

	bool operator<(const MDay & left, const MDay & right) {
		return left.d_day < right.d_day;
	}

	bool operator==(const MDay & left, const MDay & right) {
		return left.d_day == right.d_day;
	}

	ostream & operator<<(ostream & os, const MDay & day) {
		os << day.d_day;
		return os;
	}

	bool operator<(const MDate & left, const MDate & right) {
		return make_tuple(left.d_year, left.d_month, left.d_day) < make_tuple(right.d_year, right.d_month, right.d_day);
	}

	bool operator==(const MDate & left, const MDate & right) {
		return make_tuple(left.d_year, left.d_month, left.d_day) == make_tuple(right.d_year, right.d_month, right.d_day);
	}

	istream & operator >> (istream & is, MDate & date) {
		string userInput;
		copy(istream_iterator<char>(is), istream_iterator<char>(), back_inserter(userInput));
		stringstream ss(userInput);

		int year{ 0 }, month{ 0 }, day{ 0 };
		date.readSingleValue(ss, year, userInput);
		date.readSingleValue(ss, month, userInput);
		ss >> day;

		if (ss.rdbuf()->in_avail() != 0) {
			throw invalid_argument("Wrong date format: " + userInput);
		}

		MDate tmp(year, month, day);
		std::swap(date, tmp);

		return is;
	}

	ostream & operator<<(ostream & os, const MDate & date) {
		os << date.d_year << '-' << date.d_month << '-' << date.d_day;
		return os;
	}

	MYear::MYear() : d_year(0) {}

	MYear::MYear(int year) : d_year(year) {}

	MMonth::MMonth() : d_month(1) {}

	MMonth::MMonth(int month) {
		if (month < 1 || month > 12) {
			throw invalid_argument("Month value is invalid: " + std::to_string(month));
		}
		d_month = month;
	}

	MDay::MDay() : d_day(1) {}

	MDay::MDay(int day) {
		if (day < 1 || day > 31) {
			throw invalid_argument("Month value is invalid: " + std::to_string(day));
		}
		d_day = day;
	}

	MDate::MDate() : d_year(0), d_month(0), d_day(0) {}

	MDate::MDate(int year, int month, int day) : d_year(year), d_month(month), d_day(day) {}

	void MDate::readSingleValue(stringstream & ss, int & value, const string & userInput) {
		ss >> value;
		if (ss.peek() != '-') {
			throw invalid_argument("Wrong date format: " + userInput);
		}
		ss.ignore();
	}

	void MDataBase::Add(const MDate & i_date, const Event & i_event) {
		d_body[i_date].insert(i_event);
	}

	void MDataBase::Print() const {
		for (const auto& p : d_body) {
			for (const auto& ev : p.second) {
				cout << p.first << " " << ev << '\n';
			}
		}
	}

	void MDataBase::Del(const MDate & i_date, const Event & i_event) {
		auto dateIter = d_body.find(i_date);
		if (dateIter != d_body.end()) {
			auto eventIter = dateIter->second.find(i_event);
			if (eventIter != dateIter->second.end()) {
				dateIter->second.erase(eventIter);
				std::cout << "Deleted successfully";
			}
			else {
				throw invalid_argument("Event not found");
			}
		}
		else {
			throw invalid_argument("Event not found");
		}
	}

	void MDataBase::Del(const MDate & i_date) {
		auto dateIter = d_body.find(i_date);
		if (dateIter != d_body.end()) {
			cout << "Deleted " << dateIter->second.size() << " events";
			d_body.erase(dateIter);
		}
		cout << "Deleted 0 events";
	}

	void MDataBase::Find(const MDate & i_date) const {
		auto dateIter = d_body.find(i_date);
		if (dateIter != d_body.end()) {
			copy(
				dateIter->second.begin(),
				dateIter->second.end(),
				ostream_iterator<std::string>(cout, "\n")
			);
		}
		else {
			cout << "Event not found\n";
		}

	}
}