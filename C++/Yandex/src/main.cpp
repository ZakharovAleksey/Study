#include <time_logger.h>
#include <unit_test.h>

using namespace std;
using namespace unit_test;
using namespace log_time;

#include <algorithm>
#include <numeric>
#include <set>
#include <unordered_map>

template<typename Iterator>
class IteratorRange {
  public:
  IteratorRange(Iterator begin, Iterator end) : first(begin), last(end) {}

  Iterator begin() const
  {
    return first;
  }

  Iterator end() const
  {
    return last;
  }

  private:
  Iterator first, last;
};

template<typename Collection>
auto Head(Collection& v, size_t top)
{
  return IteratorRange{ v.begin(), next(v.begin(), min(top, v.size())) };
}

struct Person
{
  string name;
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

struct CmpPersonByAge
{
  bool operator()(const Person& left, const Person& right)
  {
    return left.age < right.age;
  }
};

struct CmpPersonByIncome
{
  bool operator()(const Person& left, const Person& right)
  {
    return left.income > right.income;
  }
};

std::string findPopularName(
    const std::unordered_map<std::string, size_t>& i_con)
{
  if (i_con.empty())
    return "";
  auto cmpCount = [](const auto& left, const auto& right) -> bool {
    return left.second < right.second;
  };

  const size_t maxCount =
      std::max_element(std::begin(i_con), std::end(i_con), cmpCount)->second;

  std::set<std::string> peopleMaxCount;
  for (const auto& [name, count] : i_con)
  {
    if (count == maxCount)
      peopleMaxCount.insert(name);
  }

  return *std::begin(peopleMaxCount);
}

using SetByAge = std::multiset<Person, CmpPersonByAge>;
using SetByIncome = std::multiset<Person, CmpPersonByIncome>;

using MySetPair = std::pair<SetByAge, SetByIncome>;

const MySetPair ReadPeople(std::istream& i_input, std::string& io_popMaleName,
                           std::string& io_popFemaleName)
{
  size_t count;
  i_input >> count;
  Person p;

  // ToDo: change set of peoples to multiset of ages / incomes
  SetByAge personAge;
  SetByIncome personIncome;

  std::unordered_map<std::string, size_t> maleNames;
  maleNames.reserve(count);
  std::unordered_map<std::string, size_t> femaleNames;
  femaleNames.reserve(count);

  for (size_t i = 0; i < count; ++i)
  {
    i_input >> p;
    personAge.insert(p);
    personIncome.insert(p);

    if (p.is_male)
      ++maleNames[p.name];
    else
      ++femaleNames[p.name];
  }

  io_popMaleName = findPopularName(maleNames);
  io_popFemaleName = findPopularName(femaleNames);

  return { personAge, personIncome };
}

#include <fstream>

int main()
{
  ifstream in_info("input_info.txt");
  ifstream in_com("input_comands.txt");

  std::string popMaleName, popFemaleName;

  const auto [peopleAge, peopleIncom] =
      ReadPeople(in_info, popMaleName, popFemaleName);

  for (string command; in_com >> command;)
  {
    if (command == "AGE")
    {
      int adult_age;
      in_com >> adult_age;

      auto adult_begin = lower_bound(std::begin(peopleAge), std::end(peopleAge),
                                     adult_age, [](const Person& lhs, int age) {
                                       return lhs.age < age;
                                     });

      cout << "There are " << std::distance(adult_begin, end(peopleAge))
           << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY")
    {
      int count;
      in_com >> count;

      auto head = Head(peopleIncom, count);

      int totalIncome = std::accumulate(head.begin(), head.end(), 0,
                                        [](int cur, const Person& p) {
                                          return cur += p.income;
                                        });
      cout << "Top-" << count << " people have total income " << totalIncome
           << '\n';
    } else if (command == "POPULAR_NAME")
    {
      char gender;
      in_com >> gender;

      if ((gender == 'M' && popMaleName.empty()) ||
          (gender == 'W' && popFemaleName.empty()))
        std::cout << "No people of gender " << gender << '\n';
      else
        cout << "Most popular name among people of gender " << gender << " is "
             << ((gender == 'M') ? popMaleName : popFemaleName) << '\n';
    }
  }
}
