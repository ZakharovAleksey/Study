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



// --> 
struct Image {
  double quality;
  double freshness;
  double rating;
};

struct Params {
  double a;
  double b;
  double c;
};


class FunctionPart {
public:
  FunctionPart(char operation, double value) : d_operation(operation), d_value(value) {}

  double Apply(double sourceValue) const {
    if (d_operation == '+') {
      return sourceValue + d_value;
    }
    else if (d_operation == '-'){
      return sourceValue - d_value;
    }
    else if (d_operation == '*') {
      return sourceValue * d_value;
    }
    else {
      return sourceValue / d_value;
    }
  }

  void Invert() {
    if (d_operation == '+') {
      d_operation = '-';
    }
    else if (d_operation == '-'){
      d_operation = '+';
    }
    else if (d_operation == '*') {
      d_operation = '/';
    }
    else {
      d_operation = '*';
    }
  }

private:
  char d_operation;
  double d_value;
};

class Function {
public:
  void AddPart(const char operation, double value) {
    d_parts.push_back({ operation, value });
  }
  double Apply(double value) const {
    for (const auto& part : d_parts) {
      value = part.Apply(value);
    }
    return value;
  }

  void Invert() {
    for (auto & part : d_parts) {
      part.Invert();
    }

    reverse(begin(d_parts), end(d_parts));
  }

private:
  vector<FunctionPart> d_parts;
};

Function MakeWeightFunction(const Params& params,
                            const Image& image) {
  Function function;
  function.AddPart('*', params.a);
  function.AddPart('-', image.freshness * params.b);
  function.AddPart('+', image.rating * params.c);
  return function;
}

double ComputeImageWeight(const Params& params, const Image& image) {
  Function function = MakeWeightFunction(params, image);
  return function.Apply(image.quality);
}

double ComputeQualityByWeight(const Params& params,
                              const Image& image,
                              double weight) {
  Function function = MakeWeightFunction(params, image);
  function.Invert();
  return function.Apply(weight);
}

#include<fstream>
#include <iomanip> 


struct Student {
  std::string d_fName;
  std::string d_lName;
  long int d_day;
  long int d_month;
  long int d_year;

  friend istream& operator>>(std::istream &is, Student &st) {
    is >> st.d_fName >> st.d_lName >> st.d_day >> st.d_month >> st.d_year;
    return is;
  }

  void PrintName() const {
    cout << d_fName << " " << d_lName << endl;
  }

  void PrintDate() const {
    cout << d_day << "." << d_month << "." << d_year << endl;
  }

};


class Rational {
public:
  Rational() : d_numenator(0), d_denominator(1) {}
  Rational(int numerator, int denominator) {
    if (denominator == 0) {
      throw invalid_argument("Invalid argument");
    }
    if (numerator == 0) {
      d_numenator = 0;
      d_denominator = 1;
    }
    else if (numerator < 0 && denominator < 0) {
      d_numenator = -numerator;
      d_denominator = -denominator;
    }
    else if (numerator < 0 || denominator < 0) {
      d_numenator = -abs(numerator);
      d_denominator = abs(denominator);
    }
    else {
      d_numenator = numerator;
      d_denominator = denominator;
    }

    int d = Divisor(abs(d_numenator), abs(d_denominator));
    d_numenator /= d;
    d_denominator /= d;
  }

  int Numerator() const {
    return d_numenator;
  }
  int Denominator() const {
    return d_denominator;
  }


  friend istream& operator>>(istream & is, Rational& r) {
    int num{ 0 }, denum{ 0 };
    is >> num; is.ignore(1); is >> denum;

    if (denum != 0) {
      Rational tmp{ num, denum };
      r.d_numenator = tmp.d_numenator;
      r.d_denominator = tmp.d_denominator;
    }
    
    return is;
  }

  friend ostream& operator<<(ostream& os, const Rational& r) {
    os << r.Numerator() << '/' << r.Denominator();
    return os;
  }


private:

  int Divisor(int numenator, int denumenator) {
    while (numenator > 0 && denumenator > 0) {
      if (numenator > denumenator) {
        numenator %= denumenator;
      }
      else {
        denumenator %= numenator;
      }
    }
    return denumenator + numenator;
  }

  int d_numenator;
  int d_denominator;

};

bool operator==(const Rational& left, const Rational& right) {
  return left.Numerator() == right.Numerator() && left.Denominator() == right.Denominator();
}

Rational operator+(const Rational& left, const Rational& right) {
  return {
    left.Numerator() * right.Denominator() + right.Numerator() * left.Denominator(),
    left.Denominator() * right.Denominator()
  };
}

Rational operator-(const Rational& left, const Rational& right) {
  return {
    left.Numerator() * right.Denominator() - right.Numerator() * left.Denominator(),
    left.Denominator() * right.Denominator()
  };
}

Rational operator*(const Rational& left, const Rational& right) {
  return {
    left.Numerator() * right.Numerator(),
    left.Denominator() * right.Denominator()
  };
}

Rational operator/(const Rational& left, const Rational& right) {
  if (right.Numerator() == 0) {
    throw domain_error("Division by zero");
  }
  return {
    left.Numerator() * right.Denominator(),
    left.Denominator() * right.Numerator()
  };
}

bool operator<(const Rational& left, const Rational& right) {
  return left.Numerator() * right.Denominator() < right.Numerator() * left.Denominator();
}


void EnsureEqual(const string& left, const string& right) {
  if (left != right) {
    stringstream ss;
    ss << left << " != " << right;
    throw runtime_error(ss.str());
  }
}

void Calculator() {
  int a{ 0 }, b{ 0 }, c{ 0 }, d{ 0 };
  char operation{ ' ' };
  cin >> a; cin.ignore(1);
  cin >> b; cin.ignore(1);
  cin >> operation; cin.ignore();
  cin >> c; cin.ignore(1);
  cin >> d;

  try {
    Rational first{ a, b };
    Rational second{ c, d };

    switch (operation)
    {
    case '+':
      cout << first + second;
      break;
    case '-':
      cout << first - second;
      break;
    case '*':
      cout << first * second;
      break;
    case '/':
      cout << first / second;
      break;
    default:

      break;
    };
  }
  catch (exception& e) {
    cout << e.what();
  }
  
}

string AskTimeServer() {
  return "lol";
}

class TimeServer {
public:
  string GetCurrentTime() {
    string result{ "" };
    try {
      result = AskTimeServer();
      LastFetchedTime = result;
    }
    catch (system_error& e) {
      return LastFetchedTime;
    }

    return result;
  }

private:
  string LastFetchedTime = "00:00:00";
};


// Final task

struct MYear {
	int d_year;

	MYear() : d_year(0) {}
	MYear(int year) : d_year(year) {}
};

bool operator<(const MYear& left, const MYear& right) {
	return left.d_year < right.d_year;
}

bool operator==(const MYear& left, const MYear& right) {
	return left.d_year == right.d_year;
}

ostream& operator<<(ostream& os, const MYear& year) {
	os << year.d_year;
	return os;
}

struct MMonth {
	int d_month;

	MMonth() : d_month(1) {}
	MMonth(int month) {
		if (month < 1 || month > 12) {
			throw invalid_argument("Month value is invalid: " + std::to_string(month));
		}
		d_month = month;
	}
};

bool operator<(const MMonth& left, const MMonth& right) {
	return left.d_month < right.d_month;
}

bool operator==(const MMonth& left, const MMonth& right) {
	return left.d_month == right.d_month;
}

ostream& operator<<(ostream& os, const MMonth& month) {
	os << month.d_month;
	return os;
}

struct MDay {
	int d_day;

	MDay() : d_day(1) {}
	MDay(int day) {
		if (day < 1 || day > 31) {
			throw invalid_argument("Month value is invalid: " + std::to_string(day));
		}
		d_day = day;
	}
};

bool operator<(const MDay& left, const MDay& right) {
	return left.d_day < right.d_day;
}

bool operator==(const MDay& left, const MDay& right) {
	return left.d_day == right.d_day;
}

ostream& operator<<(ostream& os, const MDay& day) {
	os << day.d_day;
	return os;
}

struct MDate {
	MYear d_year;
	MMonth d_month;
	MDay d_day;

	MDate() {}
	MDate(int year, int month, int day) : d_year(year), d_month(month), d_day(day) {}

	friend istream& operator >> (istream& is, MDate& date);

private:
	void readSingleValue(stringstream& ss, int& value, const string& userInput) {
		ss >> value;
		if (ss.peek() != '-') {
			throw invalid_argument("Wrong date format: " + userInput);
		}
		ss.ignore();
	}
};


bool operator<(const MDate& left, const MDate& right) {
	return make_tuple(left.d_year, left.d_month, left.d_day) < make_tuple(right.d_year, right.d_month, right.d_day);
}

bool operator==(const MDate& left, const MDate& right) {
	return make_tuple(left.d_year, left.d_month, left.d_day) == make_tuple(right.d_year, right.d_month, right.d_day);
}

istream& operator >> (istream& is, MDate& date) {
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

ostream& operator<<(ostream& os, const MDate& date) {
	os << date.d_year << '-' << date.d_month << '-' << date.d_day;
	return os;
}

class MDataBase {
public:
	using Event = std::string;

	void Add(const MDate& i_date, const Event& i_event) {
		d_body[i_date].insert(i_event);
	}

	void Print() const {
		for (const auto& p : d_body) {
			for (const auto& ev : p.second) {
				cout << p.first << " " << ev << '\n';
			}
		}
	}

	void Del(const MDate& i_date, const Event& i_event) {
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
	
	void Del(const MDate& i_date) {
		auto dateIter = d_body.find(i_date);
		if (dateIter != d_body.end()) {
			cout << "Deleted " << dateIter->second.size() << " events";
			d_body.erase(dateIter);
		}
		cout << "Deleted 0 events";
	}

	void Find(const MDate& i_date) const {
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

private:
	map<MDate, set<Event>> d_body;
};


class CopyTest {
public:
  CopyTest(string data) : data_(data), copy_cnt(0) {
  }

  CopyTest(const CopyTest & other) {
    data_ = other.data_;
    copy_cnt = other.copy_cnt + 1;
  }

  CopyTest(CopyTest && other) noexcept {
    data_ = other.data_;
    copy_cnt = other.copy_cnt;
  }

  CopyTest& operator=(CopyTest&& other) {
    data_ = other.data_;
    copy_cnt = other.copy_cnt;
  }

  auto begin() {
    return data_.begin();
  }

  auto end() {
    return data_.end();
  }

  string data_;
  int copy_cnt;
  using value_type = typename char;
};

bool operator==(const CopyTest & lhs, const CopyTest & rhs) {
  if (lhs.copy_cnt == rhs.copy_cnt) {
    return true;
  }
  return false;
}

bool operator<(const CopyTest & lhs, const CopyTest & rhs) {
  if (lhs.copy_cnt < rhs.copy_cnt) {
    return true;
  }
}


int main() {

	return 0;
}
