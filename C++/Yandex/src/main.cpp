#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

using namespace std;


void SortByAbsValue() {
	size_t q; cin >> q;
	std::vector<int> v; v.reserve(q);
	int cur_val{ 0 };
	for (size_t i = 0; i < q; ++i) {
		cin >> cur_val;
		v.push_back(std::move(cur_val));
	}

	std::sort(begin(v), end(v), [](const int& left, const int& right)
	{
		return abs(left) < abs(right);
	});

	std::copy(begin(v), end(v), ostream_iterator<int>(std::cout, " "));
	cout << endl;
}


void SortLower() {
	size_t q{ 0 }; cin >> q;
	std::vector<std::string> v;
	v.reserve(q);
	std::string curStr{ "" };

	for (size_t i = 0; i < q; ++i) {
		cin >> curStr;
		v.push_back(std::move(curStr));
	}

	sort(begin(v), end(v), [](const std::string& left, const std::string& right) {
		std::string l{ left }, r{ right };
		std::transform(begin(l), end(l), begin(l), ::tolower);
		std::transform(begin(r), end(r), begin(r), ::tolower);

		return l < r;
	});


	std::copy(begin(v), end(v), std::ostream_iterator<std::string>(std::cout, " "));
	std::cout << std::endl;
}


class SortedStrings {
public:
	void AddString(const string& s) {
		d_body.insert(s);
	}
	vector<string> GetSortedStrings() {
		return std::vector<std::string>(begin(d_body), end(d_body));
	}
private:
	std::multiset<std::string> d_body;
};


class Person {
public:
	void ChangeFirstName(int year, const string& first_name) {
		d_firstName[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) {
		d_lastName[year] = last_name;
	}
	string GetFullName(int year) {
		auto iterFN = d_firstName.lower_bound(year);
		auto iterLN = d_lastName.lower_bound(year);

		if (iterFN == end(d_firstName) && iterLN == end(d_lastName) ){
			return "Incognito";
		}
		else if (iterFN == end(d_firstName)) {
			if (iterLN->first == year) {
				return iterLN->second + " with unknown first name";
			}
			else if (iterLN != begin(d_firstName)) {
				return prev(iterLN)->second + " with unknown first name";
			}
		}
		else if (iterLN == end(d_lastName)) {
			if (iterFN->first == year) {
				return iterFN->second + " with unknown last name";
			}
			else if (iterFN != begin(d_lastName)) {
				return prev(iterFN)->second + " with unknown last name";
			}
		}
		else {
			if (iterFN->first == year && iterLN->first == year) {
				return iterFN->second + iterLN->second;
			}
			else if (iterFN->first != year && iterFN != begin(d_firstName) && iterLN->first == year) {
				return prev(iterFN)->second + iterLN->second;
			}
			else if (iterLN->first != year && iterLN != begin(d_lastName) && iterFN->first == year) {
				return iterFN->second + prev(iterLN)->second;
			}
			else {
				return prev(iterFN)->second + prev(iterLN)->second;
			}
		}

		return " ";
	}
private:
	map<int, std::string> d_firstName;
	map<int, std::string> d_lastName;
};


int main() {
	
	Person person;

	person.ChangeFirstName(1965, "Polina");
	person.ChangeLastName(1967, "Sergeeva");
	for (int year : {1900, 1965, 1990}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeFirstName(1970, "Appolinaria");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	person.ChangeLastName(1968, "Volkova");
	for (int year : {1969, 1970}) {
		cout << person.GetFullName(year) << endl;
	}

	return 0;
	return 0;
}