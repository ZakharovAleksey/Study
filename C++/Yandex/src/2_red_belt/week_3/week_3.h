#pragma once 

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>


namespace week_3
{
	using namespace std;

#pragma region Пул объектов

	template <class T>
	class ObjectPool {
	public:
		T* Allocate()
		{
			if (!free_up_obj_.empty())
			{
				auto top_ptr = free_up_obj_.front();
				free_up_obj_.pop();

				allocated_obj_.insert(top_ptr);
				return top_ptr;
			}

			return *allocated_obj_.insert(new T());
		}


		T* TryAllocate()
		{
			if (free_up_obj_.empty())
				return nullptr;

			return Allocate();
		}

		void Deallocate(T* object)
		{
			auto it = allocated_obj_.find(object);

			if (it == end(allocated_obj_))
				throw invalid_argument("Wrong input object");
			else
			{
				free_up_obj_.push(object);
				allocated_obj_.erase(it);
			}
		}

		~ObjectPool()
		{
			for_each(begin(allocated_obj_), end(allocated_obj_), [](T* obj) {delete obj; });
			allocated_obj_.clear();

			while (!free_up_obj_.empty())
			{
				auto top_ptr = free_up_obj_.front();
				delete top_ptr;
				free_up_obj_.pop();
			}
		}

	private:
		multiset<T*> allocated_obj_;
		queue<T*> free_up_obj_;
	};

#pragma endregion

#pragma region Swap, SortPointers, ReversedCopy

	template <typename T>
	void Swap(T* first, T* second)
	{
		const T tmp = *first;
		*first = *second;
		*second = tmp;
	}

	template <typename T>
	void SortPointers(vector<T*>& pointers)
	{
		sort(begin(pointers), end(pointers), [](const T* left, const T* right)
		{
			return *left < *right;
		});
	}

	template <typename T>
	void ReversedCopy(T* source, size_t count, T* destination)
	{
		vector<T> tmp(count);
		for (size_t i = 0; i < count; ++i)
			tmp[i] = *(source + i);

		for (auto it = rbegin(tmp); it != rend(tmp); ++it)
		{
			*destination = *it;
			++destination;
		}
	}

#pragma endregion

#pragma region  Пишем свой вектор

	template <typename T>
	class SimpleVector {
	public:
		SimpleVector() : body_(nullptr), capacity_(0u), size_(0u) {}
		explicit SimpleVector(size_t size) : body_(new T[size]), capacity_(size), size_(size) {}

		~SimpleVector() {
			delete[] body_;
		}

		T& operator[](size_t index) {
			return body_[index];
		}

		T* begin() { return body_; }
		T* end() { return body_ + size_; }

		size_t Size() const {
			return size_;
		}
		size_t Capacity() const {
			return capacity_;
		}

		void PushBack(const T& value)
		{
			if (capacity_ == 0) {
				capacity_ = size_ = 1;
				body_ = new T[size_];
				*body_ = value;

				return;
			}

			if (size_ == capacity_)
			{
				T* tmp = new T[2u * capacity_];
				copy(begin(), end(), tmp);
				delete[] body_;

				body_ = tmp;
				capacity_ *= 2u;
			}

			body_[size_++] = value;
		}

	private:
		T* body_;
		size_t capacity_;
		size_t size_;
	};

#pragma endregion

}