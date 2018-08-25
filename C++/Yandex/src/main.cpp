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

void Test()
{
	// BookManager ReadingManager
	BookManager bm;
	ASSERT_EQUAL(0, bm.Cheer(5));
	bm.Read(1, 10);
	ASSERT_EQUAL(1., bm.Cheer(1));
	bm.Read(2, 5);
	bm.Read(3, 7);
	ASSERT_EQUAL(0, bm.Cheer(2));
	ASSERT_EQUAL(0.5, bm.Cheer(3));
	bm.Read(3, 10);
	ASSERT_EQUAL(0.5, bm.Cheer(3));
	bm.Read(3, 11);
	ASSERT_EQUAL(1., bm.Cheer(3));
	ASSERT_EQUAL(0.5, bm.Cheer(1));
	bm.Read(11, 10);
	bm.Read(12, 5);
	bm.Read(13, 11);
	ASSERT_EQUAL(0., bm.Cheer(2));
	ASSERT_EQUAL(0., bm.Cheer(12));
	ASSERT_EQUAL(2./5., bm.Cheer(1));
	ASSERT_EQUAL(2./5., bm.Cheer(11));
	ASSERT_EQUAL(4./5., bm.Cheer(3));
	ASSERT_EQUAL(4./5., bm.Cheer(13));
	ASSERT_EQUAL(0., bm.Cheer(5));

}


int main() {
	TestRunner tr;
	//RUN_TEST(tr, Test);

	int i = 1'000'000'000;
	cout << i << endl;

	return 0;
}
