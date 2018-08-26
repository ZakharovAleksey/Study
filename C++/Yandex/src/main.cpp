#include"unit_test\unit_test.h"
#include"2_red_belt\week_1\week_1.h"
#include"2_red_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;

#include <chrono>
#include <iostream>
#include <string>

using namespace std;
using namespace std::chrono;

class LogDuration {
public:
	explicit LogDuration(const string& msg = "")
		: message(msg + ": ")
		, start(steady_clock::now())
	{
	}

	~LogDuration() {
		auto finish = steady_clock::now();
		auto dur = finish - start;
		cerr << message
			<< duration_cast<milliseconds>(dur).count()
			<< " ms" << endl;
	}
private:
	string message;
	steady_clock::time_point start;
};

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
  LogDuration UNIQ_ID(__LINE__){message};


//--------------------------------------------------------------
using namespace week_2;



#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void TestConstruction() {
	SimpleVector<int> empty;
	ASSERT_EQUAL(empty.Size(), 0u);
	ASSERT_EQUAL(empty.Capacity(), 0u);
	ASSERT(empty.begin() == empty.end());

	SimpleVector<string> five_strings(5);
	ASSERT_EQUAL(five_strings.Size(), 5u);
	ASSERT(five_strings.Size() <= five_strings.Capacity());
	for (auto& item : five_strings) {
		ASSERT(item.empty());
	}
	five_strings[2] = "Hello";
	ASSERT_EQUAL(five_strings[2], "Hello");
}

void TestPushBack() {
	SimpleVector<int> v;
	for (int i = 10; i >= 1; --i) {
		v.PushBack(i);
		ASSERT(v.Size() <= v.Capacity());
	}
	sort(begin(v), end(v));
	for (auto i : v)
		cout << i << " ";
	cout << endl;

	const vector<int> expected = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	ASSERT_EQUAL(v.Size(), expected.size());
	ASSERT(equal(begin(v), end(v), begin(expected)));


	SimpleVector<string> lol;

}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestConstruction);
	RUN_TEST(tr, TestPushBack);
	return 0;
}