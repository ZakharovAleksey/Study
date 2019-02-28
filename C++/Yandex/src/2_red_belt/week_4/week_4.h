#pragma once

#include<unordered_map>
#include<vector>
#include<deque>
#include<array>
#include<list>

#include<algorithm>
#include<iterator>

#include<iostream>
#include <stdexcept> 

namespace week_4
{
	using namespace std;

#pragma region Спортсмены

	void SportsmenSolver() {
		size_t numb = 0; cin >> numb;

		list<int> real_queue;
		// Stores id and its position in real_que 
		// real_queue is a LIST -> so there is no iterator invalidation (we could use postions for quick insertion)
		unordered_map<int, list<int>::const_iterator> id_pos;
		const uint32_t max_size = 100'000;
		id_pos.reserve(max_size);

		uint32_t id = 0, prev_id = 0;
		for (size_t i = 0; i < numb; ++i)
		{
			cin >> id >> prev_id;
			auto prev_id_it = id_pos.find(prev_id);

			// If prev_id is not in real_queue: insert to the end
			if (prev_id_it == id_pos.end())
			{
				real_queue.push_back(id);
				auto iter_to_pos = prev(real_queue.end());
				id_pos.insert({ id, iter_to_pos });
			}
			else // Otherwise insert before prev_id player
			{
				real_queue.insert(prev_id_it->second, id);
				id_pos.insert({ id, prev(prev_id_it->second) });
			}
		}

		copy(begin(real_queue), end(real_queue), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

#pragma endregion

#pragma region StackVector

	using namespace std;

	template <typename T, size_t N>
	class StackVector {
	public:
		explicit StackVector(size_t a_size = 0) : capacity_(N) {
			if (a_size > N)
				throw invalid_argument("Wrong input size of array");
			size_ = a_size;
		}

		T& operator[](size_t index) {
			return body_[index];
		}
		const T& operator[](size_t index) const {
			return body_[index];
		}

		auto begin() {
			return body_.begin();
		}

		auto end() {
			return begin() + size_;
		}

		auto begin() const {
			return body_.begin();
		}

		auto end() const {
			return begin() + size_;
		}

		size_t Size() const {
			return size_;
		}

		size_t Capacity() const {
			return N;
		}

		void PushBack(const T& value) {
			if (size_ == capacity_)
				throw overflow_error("Could not inser to array because the used memory is full");
			else
				body_[size_++] = value;
		}

		T PopBack() {
			if (size_ == 0)
				throw  underflow_error("Could not pop from empty array");
			else
				return body_[--size_];
		}

	private:
		array<T, N> body_;
		size_t capacity_;
		size_t size_;
	};

#pragma endregion

#pragma region AirportCounter

	// Фишка в том, что у enum class можно созранить ее размер в ПОЛЕ Last_
	// И этот размер известен на этапе компиляции. 
	// !!! Поэтому можно создать массив array<size_t, enum::Last_> !!!

	// TAirport should be enum with sequential items and last item TAirport::Last_
	template <typename TAirport>
	class AirportCounter {
	public:
		AirportCounter() {
			fill(body_.begin(), body_.end(), 0u);
		}

		template <typename TIterator>
		AirportCounter(TIterator begin, TIterator end) {
			fill(body_.begin(), body_.end(), 0u);
			for_each(begin, end, [&](const TAirport & airport) {
				++body_[static_cast<size_t>(airport)];
			});
		}


		size_t Get(TAirport airport) const {
			return body_[static_cast<size_t>(airport)];
		}


		void Insert(TAirport airport) {
			++body_[static_cast<size_t>(airport)];
		}

		void EraseOne(TAirport airport) {
			--body_[static_cast<size_t>(airport)];
		}


		void EraseAll(TAirport airport) {
			body_[static_cast<size_t>(airport)] = 0u;
		}

		using Item = pair<TAirport, size_t>;
		using Items = array<Item, static_cast<size_t>(TAirport::Last_)>;

		Items GetItems() const {
			Items tmp;
			for (size_t i = 0; i < body_.size(); ++i) {
				tmp[i] = { TAirport(i), body_[i] };
			}
			return tmp;
		}

	private:
		array<size_t, static_cast<size_t>(TAirport::Last_)> body_;
	};


#pragma endregion

#pragma region Translator

	// The main idea is to store copies of <source,target> words in one unorderred map, while in
	// second unordered map for backward translation we store string_views (references) on 
	// real string from forward unordered map.

	class Translator {
	public:
		Translator() {}

		/*
		void Add(string_view source, string_view target);
		string_view TranslateForward(string_view source) const;
		string_view TranslateBackward(string_view target) const;
		*/

	private:
		unordered_map<string, string> forward_;
		//unordered_map<string_view, string_view> backward_;
	};

#pragma endregion

#pragma region Editor

	static const size_t MAX_SIZE = 1'000u; // 1'000'000u;

	class Editor {
	public:
		Editor() : cur_buf_size_(0u) {
			start_buf_.reserve(MAX_SIZE);
		}

		void Left();
		void Right();

		void Insert(char token);
		void Copy(size_t tokens);
		void Cut(size_t tokens);
		void Paste();

		string GetText() const;

	private:
		// Part of the string before the cursor
		vector<char> start_buf_;
		// Part of the string after the cursor
		deque<char> end_buf_;

		// Buffer (store cut/copy substrings of text)
		array<char, MAX_SIZE> buffer_;
		size_t cur_buf_size_;
	};


#pragma endregion

}