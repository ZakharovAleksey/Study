#include "week_3.h"

#include <algorithm>
#include <functional>

int Sum(int x, int y)
{
  return x + y;
}

string Reverse(string s)
{
  std::reverse(s.begin(), s.end());
  return s;
}

void Sort(vector<int>& nums)
{
  std::sort(nums.begin(), nums.end());
}

#include <regex>
#include <stdexcept>

#include "..\..\0_white_belt\week_3\week_3.h"

PhoneNumber::PhoneNumber(const string& international_number)
{
  string tmp(international_number);
  regex phone_number("\\+[[:digit:]]+\-[[:digit:]]+\-(.*)");

  if (regex_match(tmp, phone_number))
  {
    // Get Country code
    size_t fond_id = tmp.find('-');
    country_code_ = tmp.substr(1, fond_id - 1);
    tmp.erase(0, fond_id + 1);

    // Get City code
    fond_id = tmp.find('-');
    city_code_ = tmp.substr(0, fond_id);
    tmp.erase(0, fond_id + 1);

    // Get Local number
    local_number_ = tmp;
  } else
    throw invalid_argument("Input phone number does not match the template.");
}

string PhoneNumber::GetCountryCode() const
{
  return country_code_;
}

string PhoneNumber::GetCityCode() const
{
  return city_code_;
}

string PhoneNumber::GetLocalNumber() const
{
  return local_number_;
}

string PhoneNumber::GetInternationalNumber() const
{
  return '+' + country_code_ + '-' + city_code_ + '-' + local_number_;
}
