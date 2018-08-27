#pragma once

#include<vector>
#include<list>
#include<array>
#include<unordered_map>

#include<algorithm>
#include<iterator>

#include<iostream>
#include <stdexcept> 


namespace week_4
{
	using namespace std;

#pragma region Спортсмены

	void SportsmenSolver()
	{
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


}