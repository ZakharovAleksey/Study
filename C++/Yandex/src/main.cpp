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

int main() {
	TestRunner tr;

  StringVector in = { "weew", "bro", "code" };
  auto res = PalindromFilter(in, 2);
  copy(begin(res), end(res), std::ostream_iterator<std::string>(std::cout, " "));
  cout << endl;

	return 0;
}