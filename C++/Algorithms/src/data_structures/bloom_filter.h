#pragma once

#include<iostream>
#include<vector>
#include<cmath>

namespace bloom_filter
{
	using namespace std;
	
	template<class Key>
	class MyHash
	{
	public:
		size_t operator()(const Key & key, size_t a, size_t b)
		{
			return (a * hash<Key>()(key) + b) % mod;
		}

	private:
		size_t mod = 101;
	};

#pragma region Bloom Filter wihought Remove() func

	template<class Key, class Hash>
	class BloomFilter
	{
	public:
		BloomFilter();
		BloomFilter(size_t keys_number, double error_probability);

		virtual void Insert(const Key & value);
		virtual bool Contains(const Key & value) const;

	protected:
		size_t table_size_;
		size_t hash_num_;
	private:
		// Vector of boolean values
		vector<bool> body_;
	};

	template<class Key, class Hash>
	inline BloomFilter<Key, Hash>::BloomFilter() : table_size_(0), hash_num_(0) {}

	template<class Key, class Hash>
	inline BloomFilter<Key, Hash>::BloomFilter(size_t keys_number, double error_probability) :
		table_size_(keys_number * abs(log(error_probability)) / (log(2.) * log(2.))),
		hash_num_(table_size_ / keys_number * log(2.)),
		body_(table_size_, false) {}

	template<class Key, class Hash>
	inline void BloomFilter<Key, Hash>::Insert(const Key & value)
	{
		for (size_t i = 1; i < hash_num_; ++i)
			body_[Hash()(value, i, i + 1) % table_size_] = true;
	}

	template<class Key, class Hash>
	inline bool BloomFilter<Key, Hash>::Contains(const Key & value) const
	{
		for (size_t i = 1; i < hash_num_; ++i)
			if (!body_[Hash()(value, i, i + 1) % table_size_])
				return false;
		return true;
	}

#pragma endregion

#pragma region Bloom Filter with Remove func

	/*
		BloomFilter:
			Instead of vector<bool> we have vector<size_t> field and
			- Each insert increments appropriate idS by one
			- Each remove decrements appropriate idS by one
	*/

	template<class Key, class Hash>
	class BloomFilterRm : public BloomFilter<Key, Hash>
	{
	public:
		BloomFilterRm() : BloomFilter() {}
		BloomFilterRm(size_t keys_number, double error_probability);

		// Each word increments appropriate insert positionS by one
		void Insert(const Key & value) override;
		bool Contains(const Key & value) const override;
		// Each word decrements appropriate insert positionS by one
		void Remove(const Key & value);

	private:
		vector<size_t> body_;
	};


	template<class Key, class Hash>
	inline BloomFilterRm<Key, Hash>::BloomFilterRm(size_t keys_number, double error_probability)
	{
		table_size_ = keys_number * abs(log(error_probability)) / (log(2.) * log(2.));
		hash_num_ = table_size_ / keys_number * log(2.);
		body_.resize(table_size_, 0);
	}

	template<class Key, class Hash>
	inline void BloomFilterRm<Key, Hash>::Insert(const Key & value)
	{
		for (size_t i = 0; i < hash_num_; ++i)
			++body_[Hash()(value, i, i + 1) % table_size_];
	}

	template<class Key, class Hash>
	inline bool BloomFilterRm<Key, Hash>::Contains(const Key & value) const
	{
		for (size_t i = 0; i < hash_num_; ++i)
			if (body_[Hash()(value, i, i + 1) % table_size_] == 0)
				return false;
		return true;
	}

	template<class Key, class Hash>
	inline void BloomFilterRm<Key, Hash>::Remove(const Key & value)
	{
		for (size_t i = 0; i < hash_num_; ++i)
			if (body_[Hash()(value, i, i + 1) % table_size_] != 0)
				--body_[Hash()(value, i, i + 1) % table_size_];
	}

#pragma endregion

}

