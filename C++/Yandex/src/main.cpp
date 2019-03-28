#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

using namespace std;

int Factorial(int n) {
  if (n <= 1) {
    return 1;
  }

  return Factorial(n - 1) * n;
}

bool IsPalindrom(const std::string & str) {
  if (str.empty() || str.size() == 1) {
    return true;
  }
  
  size_t last_id = str.size() - 1;
  for (size_t i = 0; i < str.size() / 2; ++i) {
    if (str[i] != str[last_id--]) {
      return false;
    }
  }

  return true;
}

using StringVector = std::vector<std::string>;

StringVector PalindromFilter(const StringVector & words, int minLength) {
  StringVector res;
  for (const auto & w : words) {
    if (w.size() > minLength && IsPalindrom(w)) {
      res.push_back(w);
    }
  }

  return res;
}


void UpdateIfGreater(int first, int& second) {
	if (first > second) {
		second = first;
	}
}

void MoveString(StringVector& source, StringVector& destination) {
	for (auto & w : source) {
		destination.push_back(w);
	}
	source.clear();
}

void Reverse(vector<int>& v) {
	std::reverse(begin(v), end(v));
}


vector<int> Reversed(const vector<int>& v)
{
	return vector<int>(rbegin(v), rend(v));
}

int main() {
	TestRunner tr;
	vector<int> v = { 1, 2, 3, 4 };
	vector<int> r = Reversed(v);
	copy(begin(r), end(r), ostream_iterator<int>(cout, " "));
	cout << endl;

	return 0;
}