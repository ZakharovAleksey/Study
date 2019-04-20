#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

int main() {
  {
    LOG_DURATION("test");
    TestRunner tr;
  }
}
