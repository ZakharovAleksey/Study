#pragma once

#include"../unit_test/unit_test.h"

#include <iostream>
#include<fstream>

#include<vector>
#include<string>
#include<iterator>
#include<memory>

namespace data_struct
{
	using namespace std;

	size_t GornerHash(const string & str, size_t a = 127);

	/*
		Hash Table: https://ru.wikipedia.org/wiki/%D0%A5%D0%B5%D1%88-%D1%82%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0
			- Open addressing implementation 
			- In case of collision, next key position is choosen in accordace with 
				quadratic sampling approach indexing.
			- Dynamic hash table (hash table size increases twise per each realocation).
			- In this implementation Hash Table is used for string storing.
			- Add, Remove, Find, Size functions implementation.
	*/

	template<typename T>
	class HashTable
	{
	public:
		HashTable(size_t table_size = 4) : table_size_(table_size), body_(table_size_), 
			element_numb_(0.), max_load_factor_(0.75) {}

		bool AddValue(const T & value);
		bool FindValue(const T & value);
		bool RemoveValue(const T & value);
		size_t Size() const;

		template<typename U>
		friend ostream & operator<<(ostream & os, const HashTable<U> & ht);

	private:
		void Rehashing(size_t scale_factor = 2);

		int Find(const T & value);

	private:
		size_t table_size_;
		vector<T> body_;
		double element_numb_;
		const double max_load_factor_;
	};


	template<typename T>
	inline bool HashTable<T>::AddValue(const T & value)
	{
		// If value is already in hash table
		if (Find(value) != -1)
			return false;

		++element_numb_;
		if (element_numb_ / table_size_ >= max_load_factor_)
			Rehashing();

		// Make via private method
		size_t hash = GornerHash(value) % table_size_;
		for(size_t i = 1; body_[hash] != T(); ++i)
			hash = (GornerHash(value) + i * i) % table_size_;
		body_[hash] = value;

		return true;
	}

	template<typename T>
	inline bool HashTable<T>::FindValue(const T & value)
	{
		return (Find(value) != -1);
	}

	template<typename T>
	inline bool HashTable<T>::RemoveValue(const T & value)
	{
		size_t id = Find(value);
		if (id == -1)
			return false;
		body_[id] = T();
		return true;
	}

	template<typename T>
	inline size_t HashTable<T>::Size() const
	{
		return element_numb_;
	}

	template<typename T>
	inline int HashTable<T>::Find(const T & value)
	{
		if (element_numb_ == 0)
			return -1;

		// Obtain first possible position
		size_t start_hash = GornerHash(value) % table_size_;
		if (body_[start_hash] == value)
			return start_hash;
		// Loop throw all possible positions
		size_t hash = start_hash + 1;
		for (size_t i = 1; hash != start_hash; ++i)
		{
			hash = (GornerHash(value) + i * i) % table_size_;
			if (body_[hash] == value)
				return hash;
		}

		// If value is not found
		return -1;
	}

	template<typename T>
	inline void HashTable<T>::Rehashing(size_t scale_factor)
	{
		vector<T> new_hash_table(table_size_ * scale_factor, T());

		for (const auto & value : body_)
		{
			// Make via private method
			size_t hash = GornerHash(value) % (table_size_ * scale_factor);
			for (size_t i = 1; new_hash_table[hash] != T(); ++i)
				hash = (GornerHash(value) + i * i) % (table_size_ * scale_factor);
			new_hash_table[hash] = value;
		}
		
		body_.swap(new_hash_table);
		table_size_ *= scale_factor;
	}
	
	template<typename U>
	ostream & operator<<(ostream & os, const HashTable<U>& ht)
	{
		copy_if(begin(ht.body_), end(ht.body_), ostream_iterator<U>(os, "\n"), [](const U & el)
		{
			return el != U();
		});
		return os;
	}

	void HashTableTest();

	/*
		Binary Search Tree.
			- Implemention via smart pointers (shared_ptr<MyNode>).
			- Each node has NOT pointer to parent node.
			- Only Remove method implemented with recoursion! (All other without)
			- Add, Find, Remove, Count methods are implemented.
	*/

	template<typename T>
	class MyNode
	{
	public:
		T value;
		shared_ptr<MyNode> left;
		shared_ptr<MyNode> right;

		MyNode() : value(T()), left(nullptr), right(nullptr) {}
		MyNode(const T & value) : value(value), left(nullptr), right(nullptr) {}
	};

	template<typename T>
	class BinTree
	{
	public:
		using MyNodePtr = shared_ptr<MyNode<T>>;
		BinTree() : root_(nullptr) {}

		void AddValue(const T & value);
		bool FindValue(const T & value);
		void RemoveValue(const T & value);
		size_t CountValues() const;

		// Ostream operator displays BinTree in InOrder way
		template<typename U>
		friend ostream & operator<<(ostream & os, const BinTree<U> & bt);

	private:
		void InOrderTraversal(ostream& os, MyNodePtr cur_node) const;

		// Find Successor node for choosen node
		MyNodePtr Successor(MyNodePtr cur_node);
		// Find pair <parent_node, search_node> with search_node value is equal to 'value' argument
		pair<MyNodePtr, MyNodePtr> Find(MyNodePtr cur_node, const T & value);
		// Recursive implementation of Remove by node function
		void Remove(MyNodePtr parent_node, MyNodePtr cur_node, const T & value);
		void CountValues(MyNodePtr cur_node, size_t & numb) const;

	private:
		MyNodePtr root_;
	};


	template<typename T>
	inline void BinTree<T>::AddValue(const T & value)
	{
		if (!root_)
		{
			root_ = make_shared<MyNode<T>>(value);
			return;
		}

		MyNodePtr cur_node = root_;
		MyNodePtr prev_node = nullptr;

		while (cur_node)
		{
			prev_node = cur_node;
			if (value < cur_node->value)
				cur_node = cur_node->left;
			else if (value > cur_node->value)
				cur_node = cur_node->right;
			else
				return;
		}

		if (value < prev_node->value)
			prev_node->left = make_shared<MyNode<T>>(value);
		else 
			prev_node->right = make_shared<MyNode<T>>(value);
	}

	template<typename T>
	inline bool BinTree<T>::FindValue(const T & value)
	{
		return Find(root_, value).first != nullptr;
	}

	template<typename T>
	inline void BinTree<T>::RemoveValue(const T & value)
	{	
		auto pair = Find(root_, value);
		MyNodePtr cur_node = pair.first, parent_node = pair.second;

		// If value is not in BinTree - nothing to remove
		if (cur_node == nullptr)
			return;

		Remove(parent_node, cur_node, value);
	}

	template<typename T>
	inline size_t BinTree<T>::CountValues() const
	{
		size_t count = 0;
		CountValues(root_, count);
		return count;
	}

	template<typename T>
	inline void BinTree<T>::InOrderTraversal(ostream & os, MyNodePtr cur_node) const
	{
		if (cur_node->left)
			InOrderTraversal(os, cur_node->left);

		os << cur_node->value << " ";

		if (cur_node->right)
			InOrderTraversal(os, cur_node->right);
	}

	template<typename T>
	inline shared_ptr<MyNode<T>> BinTree<T>::Successor(MyNodePtr cur_node)
	{
		MyNodePtr succ_node = cur_node->right;
		MyNodePtr parent_succ_node = cur_node;
		while (succ_node->left)
		{
			parent_succ_node = succ_node;
			succ_node = succ_node->left;
		}
		return parent_succ_node;
	}

	template<typename T>
	inline pair<shared_ptr<MyNode<T>>, shared_ptr<MyNode<T>>> BinTree<T>::Find(MyNodePtr cur_node, const T & value)
	{
		MyNodePtr prev_node = cur_node;
		while (cur_node)
		{
			if (value < cur_node->value)
			{
				prev_node = cur_node;
				cur_node = cur_node->left;
			}
			else if (value > cur_node->value)
			{
				prev_node = cur_node;
				cur_node = cur_node->right;
			}
			else
				return make_pair(cur_node, prev_node);
		}

		return make_pair(nullptr, nullptr);
	}

	template<typename T>
	inline void BinTree<T>::Remove(MyNodePtr parent_node, MyNodePtr cur_node, const T & value)
	{
		// It node with value is a leaf
		if (!cur_node->left && !cur_node->right)
		{
			bool is_left_node = (parent_node->left->value == value) ? true : false;
			cur_node.reset();

			if (is_left_node)
				parent_node->left = nullptr;
			else
				parent_node->right = nullptr;
		}
		// If node with value has ONLY LEFT child
		else if (cur_node->left && !cur_node->right)
		{
			parent_node->left = cur_node->left;
			cur_node.reset();
		}
		// If node with value has RIGHT child
		else if (cur_node->right)
		{
			MyNodePtr succ_parent = Successor(cur_node);

			// Case 1. If BinTree has right SubTree with empty left node
			/*
			  \
			   4 <- Remove this node ! Successor is equal to 5 (no left SubTree).
			  / \		We need to swap
				 5
				/ \
			null   6
			*/

			bool is_right_node = (cur_node == succ_parent);

			if (is_right_node)
			{
				cur_node->value = succ_parent->right->value;
				auto new_right_node = succ_parent->right->right;
				succ_parent->right = nullptr;
				succ_parent->right = new_right_node;
			}
			else
			{
				swap(cur_node->value, succ_parent->left->value);
				// Remove recursion
				Remove(succ_parent, succ_parent->left, value);
			}
		}

	}

	template<typename T>
	inline void BinTree<T>::CountValues(MyNodePtr cur_node, size_t & numb) const
	{
		if (cur_node->left)
			CountValues(cur_node->left, numb);

		++numb;

		if (cur_node->right)
			CountValues(cur_node->right, numb);
	}

	template<typename U>
	inline ostream & operator<<(ostream & os, const BinTree<U>& bt)
	{
		bt.InOrderTraversal(os, bt.root_);
		os << endl;
		return os;
	}

	void BinTreeTest();


	/*
		Deque.
			- Implementation via two-linked list with pointers to begin and end.
			- PushFront, PopFront, PushBack, PopBack functionns.
	*/

	template<typename T>
	struct DNode
	{
		T value;
		shared_ptr<DNode> prev;
		shared_ptr<DNode> next;

		DNode() : value(T()), prev(nullptr), next(nullptr) {}
		DNode(const T & value) : value(value), prev(nullptr), next(nullptr) {}
		DNode(const T & value, shared_ptr<DNode> prev, shared_ptr<DNode> next) :
			value(value), prev(prev), next(next) {}
	};

	template<typename T>
	class Deque
	{
	public:
		using DNodePtr = shared_ptr<DNode<T>>;

		Deque();

		void PushFront(const T & value);
		void PushBack(const T & value);
		T PopFront();
		T PopBack();

		template<typename U>
		friend ostream & operator<<(ostream & os, const Deque<U> & d);

	private:
		DNodePtr begin_;
		DNodePtr end_;
	};

	template<typename T>
	inline Deque<T>::Deque()
	{
		end_ = make_shared<DNode<T>>();
		begin_ = make_shared<DNode<T>>(T(), nullptr, end_);
		end_->prev = begin_;
	}

	template<typename T>
	inline void Deque<T>::PushFront(const T & value)
	{
		// begin_ -> cur_node (Insertion position) -> next_node
		DNodePtr cur = make_shared<DNode<T>>(value);
		// Connection of cur node with next node
		begin_->next->prev = cur;
		cur->next = begin_->next;
		// Connection of cur node with prev node
		begin_->next = cur;
		cur->prev = begin_;
	}

	template<typename T>
	inline void Deque<T>::PushBack(const T & value)
	{
		DNodePtr cur = make_shared<DNode<T>>(value);

		end_->prev->next = cur;
		cur->prev = end_->prev;

		end_->prev = cur;
		cur->next = end_;
	}

	template<typename T>
	inline T Deque<T>::PopFront()
	{
		if (begin_->next == end_)
			return -1;

		T value = begin_->next->value;
		// Swap pointers in appropriate order
		auto tmp = begin_->next->next;
		begin_->next = nullptr;
		tmp->prev = begin_;
		begin_->next = tmp;
		return value;
	}

	template<typename T>
	inline T Deque<T>::PopBack()
	{
		if (begin_->next == end_)
			return -1;

		T value = end_->prev->value;
		// Swap pointers in appropriate order
		auto tmp = end_->prev->prev;
		end_->prev = nullptr;
		tmp->next = end_;
		end_->prev = tmp;
		return value;
	}

	template<typename U>
	ostream & operator<<(ostream & os, const Deque<U>& d)
	{
		for (auto cur = d.begin_->next; cur != d.end_; cur = cur->next)
			os << cur->value << " ";
		os << endl;
		return os;
	}

	void DequeTest();
}
