#pragma once

#include<vector>
#include<forward_list>
#include<algorithm>
#include <iterator>
#include<map>
#include<unordered_map>
#include<tuple>


namespace BrownBeltWeek1 {
  
#pragma region Hash Table

  template <typename Type, typename Hasher>
  class HashSet {
  public:
    using BucketList = std::forward_list<Type>;

  public:
    explicit HashSet(size_t num_buckets, const Hasher& hasher = {}) :
      d_size(num_buckets), d_body(d_size), d_hash(hasher) {}

    void Add(const Type& value) {
      if (!Has(value))
        d_body[getBucketID(value)].emplace_front(value);
    }

    bool Has(const Type& value) const {
      const BucketList& bucket = d_body[getBucketID(value)];
      if (!bucket.empty())
        return std::find(std::begin(bucket), std::end(bucket), value) != std::end(bucket);
      return false;
    }

    void Erase(const Type& value) {
      BucketList& bucket = d_body[getBucketID(value)];
      if (!bucket.empty())
        bucket.remove(value);
    }

    const BucketList& GetBucket(const Type& value) const {
      return d_body[getBucketID(value)];
    }

  private:

    size_t getBucketID(const Type& i_value) const {
      return d_hash(i_value) % d_size;
    }

    size_t d_size;
    std::vector<BucketList> d_body;
    Hasher d_hash;
  };

#pragma endregion

#pragma region Set::iterator++

  struct Node {
    Node(int v, Node* p);

    int value{0};
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
  };

  Node* Successor(Node* io_rigthNode);
  bool isLeaf(Node* i_curNode);
  Node* Next(Node* node);

#pragma endregion

#pragma region Point struct integration in unordered_set

  using CoordType = int;

  struct Point3D {
    CoordType x{ 0 };
    CoordType y{ 0 };
    CoordType z{ 0 };

    bool operator==(const Point3D& i_other) const;
  };

  struct Hasher {
    size_t operator()(const Point3D& i_point) const;
  private:
    std::hash<CoordType> d_hash;
  };

#pragma endregion

#pragma region Person structure integration in unordered_set

  struct Address {
    std::string city, street;
    int building;

    bool operator==(const Address& other) const;
  };

  struct Person {
    std::string name;
    int height;
    double weight;
    Address address;


    bool operator==(const Person& other) const;
  };

  struct AddressHasher {
    size_t operator()(const Address& i_address) const;

  private:
    std::hash<std::string> d_hashStr;
    std::hash<int> d_hashInt;
  };

  struct PersonHasher {
    size_t operator()(const Person& i_person) const;

  private:
    std::hash<std::string> d_hashStr;
    std::hash<int> d_hashInt;
    std::hash<double> d_hashDouble;
    AddressHasher d_hashAddr;

    const size_t PrimVal{ 79 };
  };


#pragma endregion

#pragma region Secondary Index in DataBase

  struct Record {
    std::string id;
    std::string title;
    std::string user;
    int timestamp;
    int karma;
  };

  class Database {
  public:
    bool Put(const Record& record);

    const Record* GetById(const std::string& id) const;

    bool Erase(const std::string& id);

    template<typename Callback, typename IterType>
    void RangeLoop(IterType i_rangeBegin, IterType i_rangeEnd, Callback i_callback) const {
      for (auto iter = i_rangeBegin; iter != i_rangeEnd; ++iter) {
        if (!i_callback(iter->second->second))
          return;
      }
    }

    template <typename Callback>
    void RangeByTimestamp(int low, int high, Callback callback) const {
      const auto lowIter = d_timestepMap.lower_bound(low);
      const auto upIter = d_timestepMap.upper_bound(high);

      RangeLoop(lowIter, upIter, callback);
    }

    template <typename Callback>
    void RangeByKarma(int low, int high, Callback callback) const {
      const auto lowIter = d_karmaMap.lower_bound(low);
      const auto upIter = d_karmaMap.upper_bound(high);

      RangeLoop(lowIter, upIter, callback);
    }

    template <typename Callback>
    void AllByUser(const std::string& user, Callback callback) const {
      const auto lowerIter = d_userMap.lower_bound(user);
      const auto upperIter = d_userMap.upper_bound(user);

      RangeLoop(lowerIter, upperIter, callback);
    }

  private:

    using Table = std::unordered_map<std::string, Record>;
    using IntRecordMap = std::multimap<int, Table::iterator>;
    using StringRecordMap = std::multimap<std::string, Table::iterator>;
    using IterTuple = std::tuple<
      IntRecordMap::iterator, 
      IntRecordMap::iterator, 
      StringRecordMap::iterator
    >;

    Table d_body;

    // Secondary index storages
    IntRecordMap d_timestepMap;
    IntRecordMap d_karmaMap;
    StringRecordMap d_userMap;
    std::map<std::string, IterTuple> d_iterMap;


  };


#pragma endregion


}
