#include <time_logger.h>
#include <unit_test.h>

// using namespace std;
using namespace unit_test;
using namespace log_time;

// Web - server: https://github.com/pro100star/C-Plus-Plus-Coursera-Red-Belt

#include "2_red_belt\week_5\week_5.h"
using namespace RedBeltW5_NS;

void TestBasic()
{
  const set<string> key_words = { "yangle", "rocks", "sucks", "all" };

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "yangle 10 reasons why yangle  is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = { { "yangle", 7 },
                                      { "rocks", 2 },
                                      { "sucks", 1 } };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
  return 0;
}
