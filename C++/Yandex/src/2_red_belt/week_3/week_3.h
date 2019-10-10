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

#pragma region  Write your own vector

	template <typename T>
	class SimpleVector {
	public:
		SimpleVector() : body_(nullptr), capacity_(0u), size_(0u) {}
		explicit SimpleVector(size_t size) : body_(new T[size]), capacity_(size), size_(size) {}

		SimpleVector(const SimpleVector<T> & other) : 
			body_(new T[other.capacity_]), 
			capacity_(other.capacity_), 
			size_(other.size_) 
		{
			copy(other.begin(), other.end(), begin());
		}

		SimpleVector & operator=(const SimpleVector<T> & other)
		{
			if (this != &other) {
				SimpleVector tmp(other);
				swap(body_, tmp.body_);
				swap(capacity_, tmp.capacity_);
				swap(size_, tmp.size_);
			}

			return *this;
		}

		~SimpleVector() {
			delete[] body_;
		}

		T& operator[](size_t index) {
			return body_[index];
		}

		T* begin() { 
			return body_; 
		}

		T* const begin() const { 
			return body_;
		}

		T* end() { 
			return body_ + size_;
		}

		T* const end() const {
			return body_ + size_;
		}

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

#pragma region Односвязный список

	template <typename T>
	class LinkedList {
	public:
		struct Node {
			T value;
			Node* next = nullptr;

			Node(T _value) : value(_value), next(nullptr) {}
		};

		LinkedList() : root_(nullptr) {}

		~LinkedList() {
			while (root_) {
				PopFront();
			}
		}

		void PushFront(const T& value) {
			if (!root_)
				root_ = new Node(value);
			else
			{
				Node* new_root = new Node(value);
				new_root->next = root_;
				root_ = new_root;
			}
		}

		void InsertAfter(Node* node, const T& value) {
			if (!node) {
				PushFront(value);
				return;
			}

			Node* next_node = new Node(value);
			next_node->next = node->next;
			node->next = next_node;
		}

		void RemoveAfter(Node* node) {
			if (!node) {
				PopFront();
				return;
			}
			if (!node->next)
				return;

			Node* new_next = node->next->next;
			delete node->next;
			node->next = new_next;
		}

		void PopFront() {
			if (!root_)
				return;
			Node* old_root = root_;
			root_ = root_->next;
			delete old_root;

			if (!root_)
				root_ = nullptr;
		}

		Node* GetHead() { return root_; }
		const Node* GetHead() const { return root_; }

		template<class U>
		friend ostream & operator<<(ostream & os, const LinkedList<U> & l)
		{
			auto cur_node = l.GetHead();
			while (cur_node)
			{
				os << cur_node->value << " ";
				cur_node = cur_node->next;
			}
			os << endl;
			return os;
		}

	private:
		Node* root_ = nullptr;
	};


#pragma endregion

}