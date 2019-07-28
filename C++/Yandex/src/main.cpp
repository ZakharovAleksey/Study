#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

#include"0_white_belt\week_2\week_2.h"

//using namespace std;
using namespace unit_test;
using namespace log_time;

#include <cstdint>
#include<memory>
#include<array>


#include<forward_list>
#include<vector>
#include<unordered_map>
#include<deque>
#include <cassert>

#include"3_brown_belt\1_AssociativeContainers\AssociativeContainers.h"

#include <limits>
#include <random>
#include<unordered_set>

// Read about uniform distribution check


struct Record {
  std::string id;
  std::string title;
  std::string user;
  int timestamp;
  int karma;

  /*bool operator==(const Record& other) {
    return std::make_tuple(id, title, user, timestamp, karma) == std::make_tuple(other.id, other.title, other.user, other.timestamp, other.karma);
  }*/

};

// Ð ÐµÐ°Ð»Ð¸Ð·ÑƒÐ¹Ñ‚Ðµ ÑÑ‚Ð¾Ñ‚ ÐºÐ»Ð°ÑÑ
class Database {
public:
  bool Put(const Record& record){

    if(auto idIter = d_body.find(record.id); idIter == d_body.end()){
      auto iterBoolPair = d_body.insert({record.id, record});

      auto timeIter = d_timestepMap.insert({ record.timestamp, iterBoolPair.first });
      auto karmaIter = d_karmaMap.insert({ record.karma, iterBoolPair.first });
      auto userIter = d_userMap.insert({ record.user, iterBoolPair.first });

      d_iter.insert({ iterBoolPair.first, {timeIter, karmaIter, userIter}});
      return true;
    }
    return false;
  }


  const Record* GetById(const string& id) const {
    if (auto idIter = d_body.find(id); idIter != d_body.end()) {
      return &idIter->second;
    }
    return nullptr;
  }

  bool Erase(const string& id) {
    if (auto idIter = d_body.find(id); idIter != d_body.end()) {
      const auto iters = d_iter.at(idIter);

      d_timestepMap.erase(std::get<0>(iters));
      d_karmaMap.erase(std::get<1>(iters));
      d_userMap.erase(std::get<2>(iters));

      d_body.erase(id);
      return true;
    }

    return false;
  }

  template <typename Callback>
  void RangeByTimestamp(int low, int high, Callback callback) const {
    auto lowIter = d_timestepMap.lower_bound(low);
    auto upIter = d_timestepMap.upper_bound(high);

    for(const auto& iter = lowIter; iter != upIter; ++iter) {
      if(!callback(iter->second))
        return;
    }
  }

  template <typename Callback>
  void RangeByKarma(int low, int high, Callback callback) const {
    auto lowIter = d_karmaMap.lower_bound(low);
    auto upIter = d_karmaMap.upper_bound(high);

    for (const auto& iter = lowIter; iter != upIter; ++iter) {
      if (!callback(*iter->second))
        return;
    }
  }

  template <typename Callback>
  void AllByUser(const string& user, Callback callback) const {
    auto lowerIter = d_userMap.lower_bound(user);
    auto upperIter = d_userMap.upper_bound(user);
    
    for(const auto& iter = lowerIter; iter != upperIter; ++iter) {
      if (!callback(*iter->second))
        return;
    }

  }

private:
  
  using Table = std::unordered_map<std::string, Record>;
  using IntRecordMap = std::multimap<int, Table::iterator>;
  using StringRecordMap = std::multimap<std::string, Table::iterator>;


  Table d_body;

  IntRecordMap d_timestepMap;
  IntRecordMap d_karmaMap;
  StringRecordMap d_userMap;
  // May be add them to the Table -> because here we could not < by iterator
  std::map< Table::iterator, tuple<IntRecordMap::iterator, IntRecordMap::iterator, StringRecordMap::iterator>> d_iter;


};


void TestRangeBoundaries() {
  const int good_karma = 1000;
  const int bad_karma = -10;

  Database db;
  db.Put({ "id1", "Hello there", "master", 1536107260, good_karma });
  db.Put({ "id2", "O>>-<", "general2", 1536107260, bad_karma });

  int count = 0;
  db.RangeByKarma(bad_karma, good_karma, [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestSameUser() {
  Database db;
  db.Put({ "id1", "Don't sell", "master", 1536107260, 1000 });
  db.Put({ "id2", "Rethink life", "master", 1536107260, 2000 });

  int count = 0;
  db.AllByUser("master", [&count](const Record&) {
    ++count;
    return true;
  });

  ASSERT_EQUAL(2, count);
}

void TestReplacement() {
  const string final_body = "Feeling sad";

  Database db;
  db.Put({ "id", "Have a hand", "not-master", 1536107260, 10 });
  db.Erase("id");
  db.Put({ "id", final_body, "not-master", 1536107260, -10 });

  auto record = db.GetById("id");
  ASSERT(record != nullptr);
  ASSERT_EQUAL(final_body, record->title);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestRangeBoundaries);
  RUN_TEST(tr, TestSameUser);
  RUN_TEST(tr, TestReplacement);
  return 0;
}