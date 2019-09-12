#include "ConstQualifierAndUniquePtr.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace BrownBeltWeek3
{
#pragma region Task 1. Demographics 2.0

  struct Person
  {
    std::string name;
    int age, income;
    bool is_male;

    friend std::istream& operator>>(std::istream& is, Person& p);
  };

  std::istream& operator>>(std::istream& is, Person& p)
  {
    char gender;
    is >> p.name >> p.age >> p.income >> gender;
    p.is_male = (gender == 'M');
    return is;
  }

  const std::vector<Person> ReadPeople(std::istream& i_input)
  {
    int count;
    i_input >> count;

    std::vector<Person> result(count);
    for (Person& p : result)
      i_input >> p;

    std::sort(begin(result), std::end(result),
              [](const Person& left, const Person& right) {
                return left.age < right.age;
              });

    return result;
  }

  // Caluclates cumulative income from people sorted by income
  std::vector<int> getTotalIncome(const std::vector<Person>& p)
  {
    std::vector<int> res(p.size());
    std::vector<Person> personCopy(p);

    std::sort(std::begin(personCopy), std::end(personCopy),
              [](const Person& left, const Person& right) {
                return left.income > right.income;
              });

    res[0] = personCopy[0].income;
    for (size_t i = 1; i < p.size(); ++i)
      res[i] = res[i - 1] + personCopy[i].income;
    return res;
  }

  // Count number names in vector and find out popular one
  std::string findPopularName(std::vector<std::string>::iterator i_begin,
                              std::vector<std::string>::iterator i_end)
  {
    std::sort(i_begin, i_end);
    size_t count{ 1u }, curCount{ 0u };
    std::string popName{ "" };

    for (auto iter = i_begin; iter != i_end;)
    {
      auto iterLastSame =
          std::find_if_not(iter, i_end, [&iter](const std::string& name) {
            return name == *iter;
          });
      curCount = std::distance(iter, iterLastSame);
      if (curCount > count)
      {
        count = curCount;
        popName = *iter;
      } else if (curCount == count)
        popName = (popName.empty()) ? *iter : std::min(popName, *iter);

      iter = iterLastSame;
    }

    return popName;
  }

  using MyPair = std::pair<std::string, std::string>;

  const MyPair getPopularName(const std::vector<Person>& persons)
  {
    std::vector<std::string> peopleMale;
    std::vector<std::string> peopleFemale;
    peopleMale.reserve(persons.size());
    peopleFemale.reserve(persons.size());

    // Split incoming vector on M Vs W vectors
    for (const auto& p : persons)
    {
      if (p.is_male)
        peopleMale.push_back(p.name);
      else
        peopleFemale.push_back(p.name);
    }

    return { findPopularName(std::begin(peopleFemale), std::end(peopleFemale)),
             findPopularName(std::begin(peopleMale), std::end(peopleMale)) };
  }

  int mainBody()
  {
    const std::vector<Person> people = ReadPeople(std::cin);
    const std::vector<int> totalIncome = getTotalIncome(people);
    auto [femaleName, maleName] = getPopularName(people);

    for (std::string command; std::cin >> command;)
    {
      if (command == "AGE")
      {
        int adult_age;
        std::cin >> adult_age;

        auto adult_begin = lower_bound(begin(people), end(people), adult_age,
                                       [](const Person& lhs, int age) {
                                         return lhs.age < age;
                                       });

        std::cout << "There are " << std::distance(adult_begin, end(people))
                  << " adult people for maturity age " << adult_age << '\n';
      } else if (command == "WEALTHY")
      {
        int count;
        std::cin >> count;
        int total_income = (count == 0) ? 0 : totalIncome[count - 1];

        std::cout << "Top-" << count << " people have total income "
                  << total_income << '\n';
      } else if (command == "POPULAR_NAME")
      {
        char gender;
        std::cin >> gender;
        if ((gender == 'M' && maleName.empty()) ||
            (gender == 'W' && femaleName.empty()))
        {
          std::cout << "No people of gender " << gender << '\n';
        } else
          std::cout << "Most popular name among people of gender " << gender
                    << " is " << (gender == 'M' ? maleName : femaleName)
                    << '\n';
      }
    }

    return 0;
  }

#pragma endregion

}  // namespace BrownBeltWeek3
