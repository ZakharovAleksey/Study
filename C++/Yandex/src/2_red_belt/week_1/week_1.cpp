#include "week_1.h"

#include <algorithm>
#include <numeric>
#include <tuple>
#include <fstream>

using namespace std;

namespace week_1
{
bool operator==(const Date& left, const Date& right)
{
  return tie(left.year, left.month, left.day) ==
         tie(right.year, right.month, right.day);
}

bool operator!=(const Date& left, const Date& right)
{
  return !(left == right);
}

bool operator<(const Date& left, const Date& right)
{
  return tie(left.year, left.month, left.day) <
         tie(right.year, right.month, right.day);
}

ostream& operator<<(ostream& os, const Date& d)
{
  os << d.year;
  os << d.year << "-" << d.month << "-" << d.day << endl;
  return os;
}

istream& operator>>(istream& is, Date& d)
{
  is >> d.year;
  is.get();
  is >> d.month;
  is.get();
  is >> d.day;
  return is;
}

bool operator==(const Time& left, const Time& right)
{
  return tie(left.hours, left.minutes) == tie(right.hours, right.minutes);
}

bool operator!=(const Time& left, const Time& right)
{
  return !(left == right);
}

bool operator<(const Time& left, const Time& right)
{
  return tie(left.hours, left.minutes) < tie(right.hours, right.minutes);
}

ostream& operator<<(ostream& os, const Time& d)
{
  os << d.hours << ":" << d.minutes << endl;
  return os;
}

istream& operator>>(istream& is, Time& d)
{
  is >> d.hours;
  is.get();
  is >> d.minutes;
  return is;
}

} // namespace week_1
