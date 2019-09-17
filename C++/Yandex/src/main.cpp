#include <time_logger.h>
#include <unit_test.h>

// using namespace std;
using namespace unit_test;
using namespace log_time;

// Web - server: https://github.com/pro100star/C-Plus-Plus-Coursera-Red-Belt

#include <algorithm>
#include <future>
#include <limits>
#include <numeric>
#include <random>
#include <string>
#include <vector>

template<typename K, typename V>
class ConcurrentMap {
  struct ThreadSafeMap
  {
    std::map<K, V> d_map;
    std::mutex d_mutex;
  };

  public:
  static_assert(is_integral_v<K>, "ConcurrentMap supports only integer keys");

  struct Access
  {
    std::lock_guard<std::mutex> d_guard;
    V& ref_to_value;
  };

  explicit ConcurrentMap(size_t i_bucketCount) : d_body(i_bucketCount) {}

  Access operator[](const K& i_key)
  {
    auto& subMap = d_body[i_key % d_body.size()];
    return { std::lock_guard(subMap.d_mutex), subMap.d_map[i_key] };
  }

  std::map<K, V> BuildOrdinaryMap()
  {
    std::map<K, V> result;
    for (auto& subMap : d_body)
      for (auto& [key, value] : subMap.d_map)
      {
        std::lock_guard<std::mutex> guard(subMap.d_mutex);
        result[key] = value;
      }
    return result;
  }

  private:
  std::vector<ThreadSafeMap> d_body;
};

void RunConcurrentUpdates(ConcurrentMap<int, int>& cm, size_t thread_count,
                          int key_count)
{
  auto kernel = [&cm, key_count](int seed) {
    vector<int> updates(key_count);
    iota(begin(updates), end(updates), -key_count / 2);
    shuffle(begin(updates), end(updates), default_random_engine(seed));

    for (int i = 0; i < 2; ++i)
    {
      for (auto key : updates)
      {
        cm[key].ref_to_value++;
      }
    }
  };

  vector<future<void>> futures;
  for (size_t i = 0; i < thread_count; ++i)
  {
    futures.push_back(async(kernel, i));
  }
}

void TestConcurrentUpdate()
{
  const size_t thread_count = 3;
  const size_t key_count = 50000;

  ConcurrentMap<int, int> cm(thread_count);
  RunConcurrentUpdates(cm, thread_count, key_count);

  const auto result = cm.BuildOrdinaryMap();
  ASSERT_EQUAL(result.size(), key_count);
  for (auto& [k, v] : result)
  {
    AssertEqual(v, 6, "Key = " + to_string(k));
  }
}

void TestReadAndWrite()
{
  ConcurrentMap<size_t, string> cm(5);

  auto updater = [&cm] {
    for (size_t i = 0; i < 50000; ++i)
    {
      cm[i].ref_to_value += 'a';
    }
  };
  auto reader = [&cm] {
    vector<string> result(50000);
    for (size_t i = 0; i < result.size(); ++i)
    {
      result[i] = cm[i].ref_to_value;
    }
    return result;
  };

  auto u1 = async(updater);
  auto r1 = async(reader);
  auto u2 = async(updater);
  auto r2 = async(reader);

  u1.get();
  u2.get();

  for (auto f : { &r1, &r2 })
  {
    auto result = f->get();
    ASSERT(all_of(result.begin(), result.end(), [](const string& s) {
      return s.empty() || s == "a" || s == "aa";
    }));
  }
}

void TestSpeedup()
{
  {
    ConcurrentMap<int, int> single_lock(1);

    LOG_DURATION("Single lock");
    RunConcurrentUpdates(single_lock, 4, 50000);
  }
  {
    ConcurrentMap<int, int> many_locks(100);

    LOG_DURATION("100 locks");
    RunConcurrentUpdates(many_locks, 4, 50000);
  }
}

int main()
{
  TestRunner tr;
  RUN_TEST(tr, TestConcurrentUpdate);
  RUN_TEST(tr, TestReadAndWrite);
  RUN_TEST(tr, TestSpeedup);
}
