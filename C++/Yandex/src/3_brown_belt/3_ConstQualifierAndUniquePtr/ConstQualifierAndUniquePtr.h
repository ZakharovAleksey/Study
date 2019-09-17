#pragma once

#include <functional>
#include <future>
#include <mutex>
#include <optional>

namespace BrownBeltWeek3
{
  // Task 2. LazyValue
  template<typename T>
  class LazyValue {
    public:
    explicit LazyValue(std::function<T()> init) : d_body(init) {}

    bool HasValue() const
    {
      return d_object.has_value();
    }

    const T& Get() const
    {
      if (!d_object.has_value())
        d_object = d_body();
      return *d_object;
    }

    private:
    std::function<T()> d_body;
    mutable std::optional<T> d_object{ std::nullopt };
  };

  // Task 3. Synchronized template with constant methods

  template<typename T>
  class Synchronized {
    public:
    explicit Synchronized(T initial = T()) : value(std::move(initial)) {}

    struct Access
    {
      std::lock_guard<std::mutex> d_guard;
      T& ref_to_value;
    };

    struct ConstAccess
    {
      std::lock_guard<std::mutex> d_guard;
      const T& ref_to_value;
    };

    Access GetAccess()
    {
      return { std::lock_guard(d_mutex), value };
    }

    ConstAccess GetAccess() const
    {
      return { std::lock_guard(d_mutex), value };
    }

    private:
    T value;
    mutable std::mutex d_mutex;
  };

  // Task 4. Concurrent map with const qualifiers

  template<typename K, typename V, typename Hash = std::hash<K>>
  class ConcurrentMap {
    size_t getBucketId(const K& key) const
    {
      return hasher(key) % d_bucketCount;
    }

    public:
    using MapType = std::unordered_map<K, V, Hash>;

    struct WriteAccess
    {
      std::lock_guard<std::mutex> d_guard;
      V& ref_to_value;
    };

    struct ReadAccess
    {
      std::lock_guard<std::mutex> d_guard;
      const V& ref_to_value;
    };

    explicit ConcurrentMap(size_t bucket_count)
        : d_bucketCount(bucket_count), d_maps(d_bucketCount),
          d_mutexes(d_bucketCount)
    {
    }

    WriteAccess operator[](const K& key)
    {
      return { std::lock_guard(d_mutexes[getBucketId(key)]),
               d_maps[getBucketId(key)][key] };
    }
    ReadAccess At(const K& key) const
    {
      if (!Has(key))
        throw std::out_of_range("Out of range");
      else
        return { std::lock_guard(d_mutexes[getBucketId(key)]),
                 d_maps[getBucketId(key)].at(key) };
    }

    bool Has(const K& key) const
    {
      std::lock_guard<std::mutex> g(d_mutexes[getBucketId(key)]);
      return d_maps[getBucketId(key)].find(key) !=
             d_maps[getBucketId(key)].end();
    }

    MapType BuildOrdinaryMap() const
    {
      MapType result;
      for (size_t id = 0; id != d_bucketCount; ++id)
      {
        std::lock_guard<std::mutex> guard(d_mutexes[id]);
        for (const auto& [key, value] : d_maps[id])
          result[key] = value;
      }
      return result;
    }

    private:
    Hash hasher;
    size_t d_bucketCount{ 0u };
    std::vector<MapType> d_maps;
    mutable std::vector<std::mutex> d_mutexes;
  };

}  // namespace BrownBeltWeek3
