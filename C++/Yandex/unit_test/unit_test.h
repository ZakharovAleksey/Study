#pragma once

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace unit_test
{
  template<class T>
  std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
  {
    for (const auto& o : v)
      os << o << " ";
    os << std::endl;
    return os;
  }

  template<class Key, class Value>
  std::ostream& operator<<(std::ostream& os,
                           const std::unordered_map<Key, Value>& i_map)
  {
    for (const auto [key, value] : i_map)
      os << key << " : " << value << std::endl;
    return os;
  }

  template<class Key, class Value>
  std::ostream& operator<<(std::ostream& os, const std::map<Key, Value>& m)
  {
    for (const auto& [key, value] : m)
    {
      os << key << " : " << value << '\n';
    }
    return os;
  }

  template<class Key, class Value>
  bool operator==(const std::map<Key, Value>& left,
                  const std::map<Key, Value>& right)
  {
    for (const auto [key, value] : left)
    {
      if (auto it = right.find(key); it != right.end())
      {
        return value == it->second;
      } else
        return false;
    }
    return true;
  }

  template<class Key, class Value>
  bool operator==(const std::unordered_map<Key, Value>& left,
                  const std::unordered_map<Key, Value>& right)
  {
    for (const auto& [key, value] : left)
    {
      if (auto it = right.find(key); it != right.end())
      {
        return value == it->second;
      } else
        return false;
    }
    return true;
  }

  template<class T, class U>
  void AssertEqual(const T& left, const U& right, const std::string& hint)
  {
    if (right != left)
    {
      std::ostringstream os;
      os << left << " != " << right;
      if (!hint.empty())
        os << " Hint: " << hint;
      throw runtime_error(os.str());
    }
  }

  template<class T>
  void Assert(const T& left, const std::string& hint)
  {
    AssertEqual(left, true, hint);
  }

  class TestRunner {
    public:
    TestRunner();
    ~TestRunner();

    template<class TestFunc>
    void RunTest(TestFunc test, const std::string& test_name)
    {
      try
      {
        test();
        std::cerr << test_name << " OK" << std::endl;
      } catch (runtime_error& e)
      {
        ++fail_count_;
        std::cerr << test_name << " failed. Error: " << e.what() << std::endl;
      } catch (...)
      {
        ++fail_count_;
        std::cerr << test_name << " failed with Unknown error." << std::endl;
      }
    }

    private:
    size_t fail_count_;
  };
}  // namespace unit_test

#define ASSERT_EQUAL(left, right)                                              \
  {                                                                            \
    ostringstream os_lol;                                                      \
    os_lol << #left << " != " << #right << ", " << __FILE__ << ": line "       \
           << __LINE__ << endl;                                                \
    unit_test::AssertEqual(left, right, os_lol.str());                         \
  }

#define ASSERT(left)                                                           \
  {                                                                            \
    ostringstream os_lol;                                                      \
    os_lol << #left << " != "                                                  \
           << "true"                                                           \
           << ", " << __FILE__ << ": line " << __LINE__ << endl;               \
    unit_test::AssertEqual(left, true, os_lol.str());                          \
  }

#define RUN_TEST(test_runner, test_func)                                       \
  {                                                                            \
    test_runner.RunTest(test_func, #test_func);                                \
  }
