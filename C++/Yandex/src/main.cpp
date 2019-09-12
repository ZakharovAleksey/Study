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

struct cmpByIncome
{
  bool operator()(const int& left, const int& right)
  {
    return left > right;
  }
};

using SetByAge = std::multiset<int>;
using SetByIncome = std::multiset<int, cmpByIncome>;

using MySetPair = std::pair<SetByAge, std::vector<int>>;

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
    personAge.insert(p.age);
    personIncome.insert(p.income);

    if (p.is_male)
      ++maleNames[p.name];
    else
      ++femaleNames[p.name];
  }

  std::vector<int> totalIncome(personIncome.size());
  auto incomeIter = std::begin(personIncome);
  totalIncome[0] = *incomeIter;
  incomeIter = std::next(incomeIter);

  for (size_t id = 1; id < personIncome.size();
       ++id, incomeIter = std::next(incomeIter))
    totalIncome[id] = totalIncome[id - 1] + *incomeIter;

  io_popMaleName = findPopularName(maleNames);
  io_popFemaleName = findPopularName(femaleNames);

  return { personAge, totalIncome };
}

#include <fstream>

int main()
{
  ifstream in_info("input_info.txt");
  ifstream in_com("input_comands.txt");

  std::string popMaleName, popFemaleName;

  const auto& [peopleAge, peopleIncom] =
      ReadPeople(in_info, popMaleName, popFemaleName);

  for (string command; in_com >> command;)
  {
    if (command == "AGE")
    {
      int adult_age;
      in_com >> adult_age;

      auto adult_begin = peopleAge.lower_bound(adult_age);

      cout << "There are " << std::distance(adult_begin, end(peopleAge))
           << " adult people for maturity age " << adult_age << '\n';
    } else if (command == "WEALTHY")
    {
      int count;
      in_com >> count;
      int totalIncome = (count == 0) ? 0 : peopleIncom[count - 1];

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
