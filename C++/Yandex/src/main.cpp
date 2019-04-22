#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

#include <cstdint>
#include<memory>
#include<array>

//template <typename T>
//class PriorityCollection {
//public:
//	using Priority = size_t;
//	using Id = size_t;
//  using Pos = size_t;
//
//	static const size_t N = 1'000u;
//	
//  PriorityCollection() : d_curId(0u), d_maxPriority(0u), d_argMaxId(0) {}
//
//	Id Add(T object) {
//    d_priorityHeap[0].push_back(move(object));
//		d_IdIdentifier[d_curId] = make_pair(0, d_priorityHeap[0].size() - 1);
//
//    return d_curId++;
//	}
//
//  void FillInputIds(const size_t lastId, const size_t length) {
//    for(size_t id = 0; id < length; ++i) {
//      d_IdIdentifier[d_curId++] = make_pair(0, lastId + id);
//    }
//  }
//
//	template <typename ObjInputIt, typename IdOutputIt>
//	void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
//    const size_t length = std::distance(range_begin, range_end);
//    const size_t lastId = d_priorityHeap[0].size();
//
//    d_priorityHeap[0]{
//      make_move_iterator(range_begin),
//      make_move_iterator(range_end),
//      back_inserter(d_priorityHeap[0])
//    };
//		
//    //move(range_begin, range_end, back_inserter(d_priorityHeap[0]);
//
//    FillInputIds(lastId, length);
//	}
//
//	// Возможно вот тут переделать как-то а то дефолтный конструктор так себе
//  bool IsValid(Id id) const {
//    return (d_priorityHeap[d_IdIdentifier[id].first][d_IdIdentifier[id].second] == T()) ? true : false;
//  }
//
//	// Получить объект по идентификатору
//  const T& Get(Id id) const {
//    return move(d_priorityHeap[d_IdIdentifier[id]][d_IdIdentifier[id].second]);
//  }
//
//	// Увеличить приоритет объекта на 1
//  void Promote(Id id) {
//    Priority newPriority = d_IdIdentifier[id].first + 1;
//    d_priorityHeap[newPriority].push_back(move(d_priorityHeap[d_IdIdentifier[id].first][d_IdIdentifier[id].second]));
//    d_IdIdentifier[id] = make_pair(newPriority, d_priorityHeap[newPriority].size() - 1);
//    d_maxPriority = std::max(d_maxPriority, newPriority);
//    d_argMaxId = std::max(d_argMaxId, id);
//  }
//
//	// Получить объект с максимальным приоритетом и его приоритет
//  pair<const T&, int> GetMax() const {
//    return { d_priorityHeap[d_maxPriority][d_IdIdentifier[d_argMaxId].second], d_maxPriority };
//  }
//
//	// Аналогично GetMax, но удаляет элемент из контейнера
//  pair<T, int> PopMax() {
//    return { move(d_priorityHeap[d_maxPriority][d_IdIdentifier[d_argMaxId].second]), d_maxPriority };
//  }
//
//private:
//  Id d_curId;
//  Priority d_maxPriority;
//  Id d_argMaxId;
//
//	map<Priority, vector<T>> d_priorityHeap;
//	array< pair<Priority, Pos>, N> d_IdIdentifier;
//};

template <typename T>
class PriorityCollection {
public:
  using Priority = size_t;
  using Id = /* Ñ‚Ð¸Ð¿, Ð¸ÑÐ¿Ð¾Ð»ÑŒÐ·ÑƒÐµÐ¼Ñ‹Ð¹ Ð´Ð»Ñ Ð¸Ð´ÐµÐ½Ñ‚Ð¸Ñ„Ð¸ÐºÐ°Ñ‚Ð¾Ñ€Ð¾Ð² */;

  static const size_t N = 1'000'000;

  PriorityCollection() : d_curId(0), d_isValid(N, false) {
    d_body.reserve(N);
  }

  Id Add(T object) {
    d_body.push_back(move(object));
    return d_curId++;
  }

  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end, IdOutputIt ids_begin) {
    for (auto iter = range_begin; iter != range_end; ++it) {
      *(ids_begin++) = Add(*iter);
    }
  }

  bool IsValid(Id id) const {
    return d_isValid[id];
  }

  const T& Get(Id id) const {
    return d_body[id];
  }

  void Promote(Id id) {
    
  }

  pair<const T&, int> GetMax() const;

  pair<T, int> PopMax();

private:
  size_t d_curId;
  vector<bool> d_isValid;
  vector<T> d_body;
  set<pair<Priority, Id>> d_heap;
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

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}