#include "week_2.h"

namespace WhiteBeltWeek2
{
  ToDoList::ToDoList() : d_curMonthIter(begin(d_daysPerMonth))
  {
    d_body.resize(*d_curMonthIter);
  }

  void ToDoList::Add(const size_t d_dayId, const string& i_toDoItem)
  {
    d_body[d_dayId - 1].push_back(i_toDoItem);
  }

  void ToDoList::goToNextMonth()
  {
    auto nextMonthIter = next(d_curMonthIter);
    // Go to new year
    if (nextMonthIter == end(d_daysPerMonth))
    {
      nextMonthIter = begin(d_daysPerMonth);
    }
    // If number of days is the same - do nothing
    if (nextMonthIter == d_curMonthIter)
    {
      return;
    }

    if (*nextMonthIter > *d_curMonthIter)
    {
      // Update ToDo list info
      d_body.resize(*nextMonthIter);
      d_curMonthIter = nextMonthIter;
    } else
    {
      // Store all ToDo items in month
      std::vector<std::string> allMonthToDoItems;
      std::for_each(begin(d_body) + *nextMonthIter, end(d_body),
                    [&allMonthToDoItems](std::vector<std::string>& v) {
                      allMonthToDoItems.insert(allMonthToDoItems.end(),
                                               make_move_iterator(v.begin()),
                                               make_move_iterator(v.end()));
                      v.clear();
                    });
      // Update ToDo list info & add all ToDo intems to the last day of this
      // month
      d_curMonthIter = nextMonthIter;
      d_body.resize(*d_curMonthIter);
      d_body.back().insert(d_body.back().end(),
                           make_move_iterator(allMonthToDoItems.begin()),
                           make_move_iterator(allMonthToDoItems.end()));
    }
  }

  void ToDoList::Dump(const size_t i_dayId) const
  {
    cout << d_body[i_dayId - 1].size() << " ";
    copy(begin(d_body[i_dayId - 1]), end(d_body[i_dayId - 1]),
         std::ostream_iterator<std::string>(std::cout, " "));
    cout << endl;
  }

  int Factorial(int n)
  {
    if (n <= 1)
      return 1;
    return Factorial(n - 1) * n;
  }

  bool IsPalindrom(const std::string& str)
  {
    if (str.empty() || str.size() == 1)
    {
      return true;
    }

    size_t last_id = str.size() - 1;
    for (size_t i = 0; i < str.size() / 2; ++i)
    {
      if (str[i] != str[last_id--])
      {
        return false;
      }
    }

    return true;
  }

  StringVector PalindromFilter(const StringVector& words, int minLength)
  {
    StringVector res;
    for (const auto& w : words)
    {
      if (w.size() > minLength && IsPalindrom(w))
      {
        res.push_back(w);
      }
    }

    return res;
  }

  void UpdateIfGreater(int first, int& second)
  {
    if (first > second)
    {
      second = first;
    }
  }

  void MoveString(StringVector& source, StringVector& destination)
  {
    for (auto& w : source)
    {
      destination.push_back(w);
    }
    source.clear();
  }

  void Reverse(vector<int>& v)
  {
    std::reverse(begin(v), end(v));
  }

  void AvgTemperature()
  {
    int n{ 0 }, avg{ 0 };
    cin >> n;
    vector<int> tempr(n);
    vector<size_t> hot_day_ids;

    for (auto& t : tempr)
    {
      cin >> t;
      avg += t;
    }
    avg /= n;

    for (size_t i = 0; i < tempr.size(); ++i)
    {
      if (tempr[i] > avg)
      {
        hot_day_ids.push_back(i);
      }
    }

    cout << hot_day_ids.size() << endl;
    copy(begin(hot_day_ids), end(hot_day_ids),
         ostream_iterator<int>(cout, " "));
  }

  void GetQueue()
  {
    int q{ 0 };
    cin >> q;
    vector<bool> queue;
    string command{ "" };
    int input_value{ 0 };

    for (int i = 0; i < q; ++i)
    {
      std::cin >> command;

      if (command == "WORRY_COUNT")
        cout << count(begin(queue), end(queue), true) << endl;
      else
      {
        cin >> input_value;
        if (command == "WORRY")
          queue[input_value] = true;
        else if (command == "QUIET")
          queue[input_value] = false;
        else if (command == "COME")
        {
          if (input_value > 0)
            for (int i = 0; i < input_value; ++i)
              queue.push_back(false);
          else
            for (int i = 0; i < -input_value; ++i)
              queue.pop_back();
        }
      }
    }
  }

  vector<int> Reversed(const vector<int>& v)
  {
    return vector<int>(rbegin(v), rend(v));
  }

  bool IsAnagram(const std::string& first, const std::string& second)
  {
    if (first.size() != second.size())
    {
      return false;
    }

    map<char, int> m;
    for (const auto& ch : first)
    {
      ++m[ch];
    }

    for (const auto& ch : second)
    {
      auto iter = m.find(ch);
      if (iter == end(m))
      {
        return false;
      }

      if (--iter->second == 0)
      {
        m.erase(ch);
      }
    }

    return (m.empty()) ? true : false;
  }

  set<string> BuildMapValuesSet(const map<int, string>& m)
  {
    set<string> s;
    for (const auto& p : m)
    {
      s.insert(p.second);
    }
    return s;
  }

  void CountryDictionary::changeCapital(const std::string& country,
                                        const std::string& new_capital)
  {
    auto iter = d_CountryCapital.find(country);

    if (iter == end(d_CountryCapital))
    {
      d_CountryCapital[country] = new_capital;
      cout << "Introduce new country " << country << " with capital "
           << new_capital << endl;
    } else
    {
      if (iter->second == new_capital)
      {
        cout << "Country" << country << " hasn't changed its capital" << endl;
      } else
      {
        cout << "Country " << country << " has changed its capital from "
             << iter->second << " to " << new_capital << endl;
        iter->second = new_capital;
      }
    }
  }

  void CountryDictionary::renameCountry(const std::string& old_country,
                                        const std::string& new_country)
  {
    auto iterOld = d_CountryCapital.find(old_country);
    auto iterNew = d_CountryCapital.find(new_country);

    if ((old_country == new_country) || iterOld == end(d_CountryCapital) ||
        iterNew != end(d_CountryCapital))
    {
      cout << "Incorrect rename, skip" << endl;
      return;
    }

    if (iterOld != end(d_CountryCapital))
    {
      d_CountryCapital[new_country] = iterOld->second;
      cout << "Country " << old_country << " with capital " << iterOld->second
           << " has been renamed to " << new_country << endl;
      d_CountryCapital.erase(old_country);
    }
  }

  void CountryDictionary::aboutCountry(const std::string& country) const
  {
    auto iter = d_CountryCapital.find(country);

    if (iter == end(d_CountryCapital))
    {
      cout << "Country " << country << " doesn't exist" << endl;
    } else
    {
      cout << "Country " << country << " has capital " << iter->second << endl;
    }
  }

  void CountryDictionary::dumpCountry() const
  {
    if (d_CountryCapital.empty())
    {
      cout << "There are no countries in the world" << endl;
    } else
    {
      std::for_each(begin(d_CountryCapital), end(d_CountryCapital),
                    [&](const std::pair<string, string>& p) {
                      cout << p.first << " " << p.second << endl;
                    });
    }
  }

  void BusStops::newBus(const std::string& i_bus, const MyVector& i_busStops)
  {
    d_route[i_bus] = i_busStops;
    std::for_each(begin(i_busStops), end(i_busStops),
                  [&](const std::string& stop) {
                    d_stops[stop].push_back(i_bus);
                  });
  }

  void BusStops::busesForStop(const std::string& i_stop) const
  {
    auto iter = d_stops.find(i_stop);
    if (iter == end(d_stops))
    {
      cout << "No stop" << endl;
      return;
    }

    copy(begin(iter->second), end(iter->second),
         ostream_iterator<std::string>(std::cout, " "));
    cout << endl;
  }

  void BusStops::stopsForBus(const std::string& i_bus) const
  {
    auto iter = d_route.find(i_bus);

    if (iter == end(d_route))
    {
      std::cout << "No bus" << std::endl;
      return;
    }

    for (const auto& stop : iter->second)
    {
      cout << "Stop " << stop << ": ";
      auto buses = d_stops.find(stop);

      if (buses->second.size() == 1)
      {
        std::cout << "no interchange" << std::endl;
      } else
      {
        copy_if(begin(buses->second), end(buses->second),
                std::ostream_iterator<std::string>(std::cout, " "),
                [&i_bus](const std::string& cur_bus) -> bool {
                  return cur_bus != i_bus;
                });
        std::cout << std::endl;
      }
    }
  }

  void BusStops::allBuses() const
  {
    if (d_route.empty())
    {
      std::cout << "No buses" << std::endl;
      return;
    }

    for (const auto& p : d_route)
    {
      cout << "Bus " << p.first << ": ";
      std::copy(begin(p.second), end(p.second),
                ostream_iterator<std::string>(std::cout, " "));
      std::cout << std::endl;
    }
  }

  BusRouteNumber::BusRouteNumber() : d_curIndex(1) {}

  void BusRouteNumber::addRoute(const std::vector<std::string>& route)
  {
    auto iter = d_body.find(route);

    if (iter != end(d_body))
    {
      cout << "Already exists for " << iter->second << endl;
    } else
    {
      cout << "New bus " << d_curIndex << endl;
      d_body[route] = d_curIndex++;
    }
  }

  void Synonim::add(const string& first, const string& second)
  {
    d_body[first].insert(second);
    d_body[second].insert(first);
  }

  int Synonim::getSynonimsCount(const string& word) const
  {
    auto iter = d_body.find(word);

    if (iter == end(d_body))
    {
      return 0;
    } else
    {
      return iter->second.size();
    }
  }

  bool Synonim::areSynonims(const string& first, const string& second) const
  {
    auto iter = d_body.find(first);
    if (iter != end(d_body))
    {
      auto iter2 = iter->second.find(second);
      return (iter2 == end(iter->second)) ? false : true;
    }
    return false;
  }

  BusRouteNumber2::BusRouteNumber2() : d_curIndex(1) {}

  void BusRouteNumber2::AddRoute(const std::set<std::string>& route)
  {
    auto iter = d_body.find(route);

    if (iter != end(d_body))
    {
      cout << "Already exists for " << iter->second << endl;
    } else
    {
      cout << "New bus " << d_curIndex << endl;
      d_body[route] = d_curIndex++;
    }
  }
}  // namespace WhiteBeltWeek2
