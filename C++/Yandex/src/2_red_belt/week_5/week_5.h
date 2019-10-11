#pragma once

#include <algorithm>
#include <future>
#include <iterator>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include "../week_1/week_1.h"

namespace RedBeltW5_NS
{
  using namespace std;

  // Task 1. Josephus permutation without copy
  template<typename RandomIt>
  void MakeJosephusPermutation(RandomIt first, RandomIt last,
                               uint32_t step_size)
  {
    vector<typename RandomIt::value_type> pool;
    move(first, last, std::back_inserter(pool));

    size_t cur_pos{ 0u }, count{ 0u },
        size{ static_cast<size_t>(std::distance(first, last)) };
    vector<bool> isKilled(size, false);

    while (true)
    {
      *first++ = move(pool[cur_pos]);
      isKilled[cur_pos] = true;

      if (++count == size)
      {
        break;
      }
      // Here we caclulate position of next dead man skipping already killed
      // and cheking boundaries
      for (size_t cur_shift = 0; cur_shift < step_size;)
      {
        cur_pos = (++cur_pos == size) ? 0 : cur_pos;
        if (!isKilled[cur_pos])
        {
          ++cur_shift;
        }
      }
    }
  }

  // Task 2. Split string without copy

  template<typename String>
  using Group = vector<String>;

  template<typename String>
  using Char = typename String::value_type;

  template<typename String>
  vector<Group<String>> GroupHeavyStrings(vector<String> strings)
  {
    map<set<Char<String>>, Group<String>> groupMap;
    set<Char<String>> curKey;

    for (auto& curString : strings)
    {
      std::for_each(begin(curString), end(curString),
                    [&curKey](const Char<String>& symbol) {
                      curKey.insert(symbol);
                    });
      auto keyIter = groupMap.find(curKey);
      if (keyIter != groupMap.end())
      {
        keyIter->second.push_back(move(curString));
      } else
      {
        groupMap[move(curKey)].push_back(move(curString));
      }
      curKey.clear();
    }
    vector<Group<String>> res;
    res.reserve(groupMap.size());
    for (auto& c : groupMap)
    {
      res.push_back(move(c.second));
    }
    return move(res);
  }

  // Task 3. Split text into sentences without copy
  // Hint: Main idea here to get to TIME LIMIT: allocate memory for vector &
  // move element by element, not whole vector at once and call clear after
  // move. Than capacity will not change and there will be no reallocations.

  template<typename Token>
  using Sentence = vector<Token>;

  template<typename Token>
  vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens)
  {
    vector<Sentence<Token>> result;
    if (tokens.empty())
    {
      return result;
    }
    bool isPrevEnd = tokens[0].IsEndSentencePunctuation();

    Sentence<Token> sentence;
    sentence.reserve(1'000'000);
    sentence.push_back(move(tokens[0]));

    for (auto tokenIter = make_move_iterator(next(begin(tokens)));
         tokenIter != make_move_iterator(end(tokens)); ++tokenIter)
    {
      bool isCurEnd = tokenIter->IsEndSentencePunctuation();

      if (isPrevEnd && !isCurEnd)
      {
        // Create new sentence and move all elements from current sentence
        result.push_back({});
        result.back().reserve(sentence.size());

        for (auto& word : sentence)
        {
          result.back().push_back(move(word));
        }

        sentence.clear();
        sentence.push_back(*tokenIter);
      } else
      {
        sentence.push_back(*tokenIter);
      }
      isPrevEnd = isCurEnd;
    }

    if (!sentence.empty())
    {
      result.push_back(move(sentence));
    }
    return result;
  }

  // Task 4. Simple vector with move push_back

  template<typename T>
  class MoveSimpleVector {
    public:
    MoveSimpleVector() : body_(nullptr), capacity_(0u), size_(0u) {}
    explicit MoveSimpleVector(size_t size)
        : body_(new T[size]), capacity_(size), size_(size)
    {
    }

    ~MoveSimpleVector()
    {
      delete[] body_;
    }

    T& operator[](size_t index)
    {
      return body_[index];
    }

    T* begin()
    {
      return body_;
    }

    T* const begin() const
    {
      return body_;
    }

    T* end()
    {
      return body_ + size_;
    }

    T* const end() const
    {
      return body_ + size_;
    }

    size_t Size() const
    {
      return size_;
    }
    size_t Capacity() const
    {
      return capacity_;
    }

    void PushBack(T value)
    {
      if (capacity_ == 0)
      {
        capacity_ = size_ = 1;
        body_ = new T[size_];
        *body_ = std::move(value);

        return;
      }

      if (size_ == capacity_)
      {
        T* tmp = new T[2u * capacity_];
        move(begin(), end(), tmp);
        delete[] body_;

        body_ = tmp;
        capacity_ *= 2u;
      }

      body_[size_++] = std::move(value);
    }

    private:
    T* body_;
    size_t capacity_;
    size_t size_;
  };

  // Task 5. 3-way merge sort via move semantics

  template<typename RandomIt>
  void MergeSort(RandomIt range_begin, RandomIt range_end)
  {
    size_t length = std::distance(range_begin, range_end);
    if (length < 2)
    {
      return;
    }

    vector<typename RandomIt::value_type> res{ make_move_iterator(range_begin),
                                               make_move_iterator(range_end) };

    auto iterEndFirst{ res.begin() + length / 3 };
    auto iterEndSecond{ res.begin() + 2 * length / 3 };

    MergeSort(res.begin(), iterEndFirst);
    MergeSort(iterEndFirst, iterEndSecond);
    MergeSort(iterEndSecond, res.end());

    vector<typename RandomIt::value_type> tmp;
    tmp.reserve(std::distance(res.begin(), iterEndFirst));

    merge(make_move_iterator(res.begin()), make_move_iterator(iterEndFirst),
          make_move_iterator(iterEndFirst), make_move_iterator(iterEndSecond),
          back_inserter(tmp));

    merge(make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
          make_move_iterator(iterEndSecond), make_move_iterator(res.end()),
          range_begin);
  }

  // Task 6. Sum of matrix via async

  template<typename ContainerOfVectors>
  int64_t CalculateSumSingleThread(ContainerOfVectors& result)
  {
    int64_t subSum{ 0 };
    for (auto& row : result)
    {
      subSum += std::accumulate(row.begin(), row.end(), 0);
    }

    return subSum;
  }

  /*int64_t CalculateMatrixSum(const vector<vector<int>>& matrix)
  {
    int64_t sum{ 0 };

    std::vector<std::future<int64_t>> futures;
    size_t pageSize{ 2250 };

    for (auto page : week_1::Paginate(matrix, pageSize))
    {
      futures.push_back(std::async([page] {
        return CalculateSumSingleThread(page);
      }));
    }
    for (auto& f : futures)
    {
      sum += f.get();
    }

    return sum;
  }*/

#pragma region ASUNC

  // Task 5. Research of blogs
  struct Stats
  {
    std::map<string, int> word_frequences;

    void operator+=(const Stats& i_other);
  };

  Stats ExploreLine(const set<string>& key_words, const std::string& line);

  Stats ExploreKeyWordsSingleThread(const set<string>& key_words,
                                    istream& input);

  Stats ExploreKeyWords(const set<string>& key_words, istream& input);

  // Task 6. Synchronized template:
  // Main idea : create for Access class lock_guard<> behavior by adding it as a
  // member of this structure.

  template<typename T>
  class Synchronized {
    public:
    explicit Synchronized(T i_initial = T()) : d_value(std::move(i_initial)) {}

    struct Access
    {
      T& ref_to_value;
      std::lock_guard<std::mutex> guard;
    };

    Access GetAccess()
    {
      return { d_value, std::lock_guard(d_mutex) };
    }

    private:
    T d_value;
    std::mutex d_mutex;
  };

  // Task 7. Concurrent map:

  template<typename K, typename V>
  class ConcurrentMap {
    // Pair of map & corresponding mutex
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

#pragma endregion

}  // namespace RedBeltW5_NS
