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
  Person() = delete;
  Person(const string& fName, const string& lName, const int year) : d_birthYear(year) {
    d_fName[d_birthYear] = fName;
    d_lName[d_birthYear] = lName;
  }

	void ChangeFirstName(int year, const string& first_name) {
    if (year < d_birthYear) {
      return;
    }
		d_fName[year] = first_name;
	}
	void ChangeLastName(int year, const string& last_name) {
    if (year < d_birthYear) {
      return;
    }
		d_lName[year] = last_name;
	}

	string GetFullName(int year) const {
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

  string GetFullNameWithHistory(int year) const {
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

private:
	map<int, std::string> d_fName;
	map<int, std::string> d_lName;

  int d_birthYear;
};


class ReversibleString {
public:
  ReversibleString() {}
  ReversibleString(const std::string & str) : d_body(str) {}

  void Reverse() {
    std::reverse(begin(d_body), end(d_body));
  }

  std::string ToString() const {
    return d_body;
  }

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

int main() {
  
  LectureTitle title(
    Specialization("C++"),
    Course("White belt"),
    Week("4th")
  );


  LectureTitle title("C++", "White belt", "4th");

  LectureTitle title(string("C++"), string("White belt"), string("4th"));

  LectureTitle title = { "C++", "White belt", "4th" };

  LectureTitle title = { {"C++"}, {"White belt"}, {"4th"} };

  LectureTitle title(
    Course("White belt"),
    Specialization("C++"),
    Week("4th")
  );

  LectureTitle title(
    Specialization("C++"),
    Week("4th"),
    Course("White belt")
  );

  return 0;
}