#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

#include <cstdint>
#include<memory>
#include<array>

template <typename T>
class PriorityCollection {
public:
  using Priority = int;
  using Id = size_t;
  using PriorIdPair = pair<Priority, Id>;

  static const size_t N = 1'000;

  PriorityCollection() : d_curId(0) {
    d_objects.reserve(N);
    std::fill(d_isObjValid.begin(), d_isObjValid.end(), false);
    std::fill(d_objPriority.begin(), d_objPriority.end(), 0);
  }

  Id Add(T object) {
    d_objects.push_back(move(object));
    d_isObjValid[d_curId] = true;
    d_heap.insert({0, d_curId});
    // Default priority is 0 (no assigment is needed)
    return d_curId++;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (auto iter = make_move_iterator(range_begin); iter != make_move_iterator(range_end); ++iter) {
      *(ids_begin++) = Add(*iter);
    }
  }

  bool IsValid(Id id) const {
    return d_isObjValid[id];
  }

  const T& Get(Id id) const {
    return move(d_objects[id]);
  }

  void Promote(Id id) {
    PriorIdPair pair = make_pair(d_objPriority[id], id);

    auto iter = d_heap.find(pair);
    if (iter != d_heap.end()) {
      PriorIdPair newPair = { (iter->first) + 1, iter->second };
      d_heap.erase(pair);
      d_heap.insert(newPair);
      d_objPriority[id] = newPair.first;
    }
  }

  pair<const T&, int> GetMax() const {
    return { d_objects[d_heap.begin()->second], d_heap.begin()->first };
  }

  pair<T, int> PopMax() {
    d_isObjValid[d_heap.begin()->second] = false;

    pair<T, int> res{ move(d_objects[d_heap.begin()->second]), d_heap.begin()->first };
    d_heap.erase(d_heap.begin());
    return res;
  }

  struct SetComparator {
    bool operator()(const PriorIdPair& left, const PriorIdPair& right) const {
      if (left.first > right.first) {
        return true;
      }
      else if (left.first == right.first) {
        return left.second > right.second;
      }
      return false;
    }

  };

private:
  size_t d_curId;
  vector<T> d_objects;
  array<bool, N> d_isObjValid;
  array<Priority, N> d_objPriority;
  set<PriorIdPair, SetComparator> d_heap;
};


class StringNonCopyable : public string {
public:
  using string::string;
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

using Priority = int;
using Id = size_t;

using PriorityIdPair = std::pair<Priority, Id>;

struct MyPairComparator {
  bool operator()(const PriorityIdPair& left, const PriorityIdPair& right) {
    if (left.first > right.first) {
      return true;
    }
    else if (left.first == right.first) {
      return left.second < right.second;
    }
    return false;
  }
};

ostream& operator<<(ostream& os, const PriorityIdPair& p) {
  os << "(" << p.first << ", " << p.second << ") ";
  return os;
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);

  /*set<PriorityIdPair, MyPairComparator> s;
  s.insert({ 1, 0 });
  s.insert({ 2, 3 });
  s.insert({ 1, 3 });
  s.insert({ 1, 1 });
  

  auto lol = *s.begin();
  s.erase(s.begin());
  cout << "TOP: " << lol.first << " " << lol.second << endl;
  
  for (const auto& i : s) {
    cout << "(" << i.first << ", " << i.second << ") ";
  }
  cout << endl;*/
  
  return 0;
}