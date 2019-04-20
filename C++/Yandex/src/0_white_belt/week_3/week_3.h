#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<string>
#include<iterator>
#include<fstream>
#include <iomanip> 

namespace WhiteBeltW3 {

	using namespace std;

	void SortByAbsValue();
	void SortLower();


	class SortedStrings {
	public:
		void AddString(const string& s);
		vector<string> GetSortedStrings();
	private:
		std::multiset<std::string> d_body;
	};


	class Person {
	public:
		Person() = delete;
		Person(const string& fName, const string& lName, const int year);

		void ChangeFirstName(int year, const string& first_name);
		void ChangeLastName(int year, const string& last_name);

		string GetFullName(int year) const;
		string GetFullNameWithHistory(int year) const;

	private:
		map<int, std::string> d_fName;
		map<int, std::string> d_lName;

		int d_birthYear;
	};


	class ReversibleString {
	public:
		ReversibleString() {}
		ReversibleString(const std::string & str) : d_body(str) {}

		void Reverse();
		std::string ToString() const;
	private:
		std::string d_body;
	};


	class Incognizable {
	public:
		Incognizable() : d_a(0), d_b(0) {}
		Incognizable(int a) : d_a(a), d_b(0) {}
		Incognizable(int a, int b) : d_a(a), d_b(b) {}
	private:
		int d_a;
		int d_b;
	};


	struct Specialization {
		string d_str;
		explicit Specialization(const string& str) : d_str(str) {}
	};

	struct Course {
		string d_str;
		explicit Course(const string& str) : d_str(str) {}
	};

	struct Week {
		string d_str;
		explicit Week(const string& str) : d_str(str) {}
	};

	struct LectureTitle {
		Specialization specialization;
		Course course;
		Week week;

		explicit LectureTitle(Specialization s, Course c, Week w) : specialization(s), course(c), week(w) {}
	};

}