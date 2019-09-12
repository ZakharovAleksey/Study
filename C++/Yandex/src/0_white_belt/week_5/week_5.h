#pragma once

#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace WhiteBeltW5
{
  using namespace std;

  struct MYear
  {
    int d_year;

    MYear();
    MYear(int year);
  };

  bool operator<(const MYear& left, const MYear& right);
  bool operator==(const MYear& left, const MYear& right);
  ostream& operator<<(ostream& os, const MYear& year);

  struct MMonth
  {
    int d_month;

    MMonth();
    MMonth(int month);
  };

  bool operator<(const MMonth& left, const MMonth& right);
  bool operator==(const MMonth& left, const MMonth& right);
  ostream& operator<<(ostream& os, const MMonth& month);

  struct MDay
  {
    int d_day;

    MDay();
    MDay(int day);
  };

  bool operator<(const MDay& left, const MDay& right);
  bool operator==(const MDay& left, const MDay& right);
  ostream& operator<<(ostream& os, const MDay& day);

  struct MDate
  {
    MYear d_year;
    MMonth d_month;
    MDay d_day;

    MDate();
    MDate(int year, int month, int day);

    friend istream& operator>>(istream& is, MDate& date);

    private:
    void readSingleValue(stringstream& ss, int& value, const string& userInput);
  };

  bool operator<(const MDate& left, const MDate& right);
  bool operator==(const MDate& left, const MDate& right);
  istream& operator>>(istream& is, MDate& date);
  ostream& operator<<(ostream& os, const MDate& date);

  class MDataBase {
    public:
    using Event = std::string;

    void Add(const MDate& i_date, const Event& i_event);
    void Del(const MDate& i_date, const Event& i_event);
    void Del(const MDate& i_date);
    void Find(const MDate& i_date) const;

    void Print() const;

    private:
    map<MDate, set<Event>> d_body;
  };
}  // namespace WhiteBeltW5
