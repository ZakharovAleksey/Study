#pragma once

#include<iostream>
#include<vector>
#include<map>
#include<set>
#include<cstring>
#include<string>
#include<algorithm>
#include<iterator>

namespace WhiteBeltWeek2 {
  using namespace std;

#pragma region functions

  int Factorial(int n);
  bool IsPalindrom(const std::string & str);

  using StringVector = std::vector<std::string>;
  StringVector PalindromFilter(const StringVector & words, int minLength);
  void UpdateIfGreater(int first, int& second);
  void MoveString(StringVector& source, StringVector& destination);

#pragma endregion

#pragma region container vector
  
  void Reverse(vector<int>& v);
  void AvgTemperature();
  void GetQueue();
  vector<int> Reversed(const vector<int>& v);

  class ToDoList {
  public:
    ToDoList();

    void Add(const size_t d_dayId, const string & i_toDoItem);
    void goToNextMonth();
    void Dump(const size_t i_dayId) const;

  private:
    std::vector<std::vector<std::string>> d_body;
    std::vector<size_t>::const_iterator d_curMonthIter;

    const std::vector<size_t> d_daysPerMonth =
    {
      31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };
  };

#pragma endregion

#pragma region container map

// Checks if the word is anagram
  bool IsAnagram(const std::string & first, const std::string& second);

  // <Country - Capital> dictionary
  class CountryDictionary {
  public:
    CountryDictionary() = default;

    void changeCapital(const std::string& country, const std::string& new_capital);
    void renameCountry(const std::string& old_country, const std::string& new_country);
    void aboutCountry(const std::string& country) const;
    void dumpCountry() const;

  private:
    std::map<std::string, std::string> d_CountryCapital;
  };

  // Bus stops task
  class BusStops {
  public:
    using MyVector = std::vector<std::string>;
    using MyMap = std::map<std::string, MyVector>;

    void newBus(const std::string& i_bus, const MyVector& i_busStops);
    void busesForStop(const std::string& i_stop) const;
    void stopsForBus(const std::string& i_bus) const;
    void allBuses() const;

  private:
    MyMap d_route;
    MyMap d_stops;
  };

  // Bus route number of 
  class BusRouteNumber {
  public:
    BusRouteNumber();

    void addRoute(const std::vector<std::string>& route);
    
  private:
    size_t d_curIndex;
    std::map<std::vector<std::string>, int> d_body;
  };

#pragma endregion

#pragma region container set

  class Synonim {
  public:
    Synonim() = default;
    void add(const string& first, const string& second);

    int getSynonimsCount(const string& word) const;
    bool areSynonims(const string& first, const string& second) const;

  private:
    std::map< std::string, std::set<string>> d_body;
  };

  set<string> BuildMapValuesSet(const map<int, string>& m);

  class BusRouteNumber2 {
  public:
    BusRouteNumber2();

    void AddRoute(const std::set<std::string>& route);

  private:
    size_t d_curIndex;
    std::map<std::set<std::string>, int> d_body;
  };

#pragma endregion
}