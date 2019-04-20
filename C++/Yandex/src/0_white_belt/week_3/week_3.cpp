#include"week_3.h"

#include<algorithm>
#include<iterator>
#include<sstream>

namespace WhiteBeltW3 {
	
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

	void Person::ChangeFirstName(int year, const string& first_name) {
		if (year < d_birthYear) {
			return;
		}
		d_fName[year] = first_name;
	}

	string Person::GetFullName(int year) const {
		if (year < d_birthYear) {
			return "No person";
		}

		auto lb_first_name = d_fName.upper_bound(year);
		auto lb_last_name = d_lName.upper_bound(year);

		if (lb_first_name == begin(d_fName) && lb_last_name == begin(d_lName))
			return "Incognito";
		else if (lb_first_name == begin(d_fName) && lb_last_name != begin(d_lName))
			return prev(lb_last_name)->second + " with unknown first name";
		else if (lb_first_name != begin(d_fName) && lb_last_name == begin(d_lName))
			return prev(lb_first_name)->second + " with unknown last name";
		else
			return prev(lb_first_name)->second + " " + prev(lb_last_name)->second;
	}

	string Person::GetFullNameWithHistory(int year) const {
		if (year < d_birthYear) {
			return "No person";
		}

		bool isFirstLast = false;

		vector<string> fName;
		vector<string> lName;
		auto iterUBFn = d_fName.upper_bound(year);
		for (auto it = d_fName.begin(); it != iterUBFn; ++it) {
			fName.push_back(it->second);
		}

		fName.erase(std::unique(begin(fName), end(fName)), end(fName));

		auto iterUBLn = d_lName.upper_bound(year);
		for (auto it = d_lName.begin(); it != iterUBLn; ++it) {
			lName.push_back(it->second);
		}

		lName.erase(std::unique(begin(lName), end(lName)), end(lName));

		stringstream ss;

		if (fName.empty() && lName.empty()) {
			ss << "Incognito";
		}
		else if (!fName.empty() && lName.empty()) {
			auto iter = rbegin(fName);
			ss << *iter++;

			if (iter != rend(fName)) {
				ss << " (";
				copy(iter, rend(fName), ostream_iterator<string>(ss, ", "));
				ss.seekp(ss.str().length() - 2);
				ss << ")";
			}
			ss << " with unknown last name";
		}
		else if (fName.empty() && !lName.empty()) {
			auto iter = rbegin(lName);
			ss << *iter++;
			if (iter != rend(lName)) {
				ss << " (";
				copy(iter, rend(lName), ostream_iterator<string>(ss, ", "));
				ss.seekp(ss.str().length() - 2);
				ss << ")";
			}
			ss << " with unknown first name";
		}
		else {
			auto iter = rbegin(fName);
			ss << *iter++;
			if (iter != rend(fName)) {
				ss << " (";
				copy(iter, rend(fName), ostream_iterator<string>(ss, ", "));
				ss.seekp(ss.str().length() - 2);
				ss << ")";
			}
			iter = rbegin(lName);
			ss << " " << *iter++;
			if (iter != rend(lName)) {
				ss << " (";
				copy(iter, rend(lName), ostream_iterator<string>(ss, ", "));
				ss.seekp(ss.str().length() - 2);
				ss << ")";
				isFirstLast = true;
			}
		}

		std::string res = ss.str();
		if (isFirstLast) {
			res.pop_back();
		}
		return res;
	}


	void Person::ChangeLastName(int year, const string& last_name) {
		if (year < d_birthYear) {
			return;
		}
		d_lName[year] = last_name;
	}

	void SortedStrings::AddString(const string & s) {
		d_body.insert(s);
	}

	vector<string> SortedStrings::GetSortedStrings() {
		return std::vector<std::string>(begin(d_body), end(d_body));
	}

	Person::Person(const string & fName, const string & lName, const int year) : d_birthYear(year) {
		d_fName[d_birthYear] = fName;
		d_lName[d_birthYear] = lName;
	}

	void ReversibleString::Reverse() {
		std::reverse(begin(d_body), end(d_body));
	}

	std::string ReversibleString::ToString() const {
		return d_body;
	}
}