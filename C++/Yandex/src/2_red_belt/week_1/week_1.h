#pragma once

#include <string>
#include <vector>
#include<tuple>


#define SORT_BY(field)													\
	[] (const AirlineTicket & left, const AirlineTicket & right){		\
		return left.field < right.field;								\
	}																	\

#define UPDATE_FIELD(ticket, field, values){	\
	auto it = values.find(#field);				\
	if (it != values.end()) {					\
		istringstream is(it->second);			\
		is >> ticket.field;						\
	}											\
}

namespace week_1
{
	using namespace std;

#pragma region Макрос UPDATE_FIELD  и Макрос SORT_BY

	struct Date {
		int year, month, day;
		friend ostream & operator<<(ostream & os, const Date & d);
		friend istream & operator >> (istream & is, Date & d);
	};

	struct Time {
		int hours, minutes;
		friend ostream & operator<<(ostream & os, const Time & d);
		friend istream & operator >> (istream & is, Time & d);
	};

	struct AirlineTicket {
		string from;
		string to;
		string airline;
		Date departure_date;
		Time departure_time;
		Date arrival_date;
		Time arrival_time;
		int price;
	};

#pragma endregion

#pragma region Таблица

	template<class T>
	class Table
	{
	public:
		Table(size_t rows, size_t columns) : rows_(rows), columns_(columns)
		{
			body_.resize(rows_, vector<T>(columns_, T()));
		}

		vector<T> & operator[](size_t id)
		{
			return body_[id];
		}

		const vector<T> & operator[](size_t id) const
		{
			return body_[id];
		}

		void Resize(size_t new_rows, size_t new_columns)
		{
			vector<vector<T>> tmp = body_;
			body_.clear();
			body_.resize(new_rows, vector<T>(new_columns, T()));

			for (size_t y = 0; y < min(rows_, new_rows); ++y)
				for (size_t x = 0; x < min(columns_, new_columns); ++x)
					body_[y][x] = tmp[y][x];

			rows_ = new_rows;
			columns_ = new_columns;
		}

		pair<size_t, size_t> Size() const
		{
			return{ rows_, columns_ };
		}

	private:

		size_t rows_;
		size_t columns_;

		vector<vector<T>> body_;
	};


#pragma endregion
	
#pragma region Дек на основе двух векторов

	template<class T>
	class Deque
	{
	public:
		Deque() { }

		bool Empty() const { 
			return front_.empty() && back_.empty(); 
		}

		size_t Size() const { 
			return front_.size() + back_.size(); 
		}

		T & operator[](size_t index)
		{
			if (index < front_.size())
				return front_[front_.size() - index - 1];
			else
				return back_[index - front_.size()];
		}

		const T & operator[](size_t index) const
		{
			if (index < front_.size())
				return front_[front_.size() - index - 1];
			else
				return back_[index - front_.size()];
		}

		T & At(size_t index)
		{
			if (index >= Size())
				throw out_of_range("Index is bigger than number of elements.");

			return this->operator[](index);
		}

		const T & At(size_t index) const
		{
			if (index >= Size())
				throw out_of_range("Index is bigger than number of elements.");

			return this->operator[](index);
		}


		T & Front() {
			return (!front_.empty()) ? *prev(end(front_)) : *begin(back_);
		}

		const T & Front() const {
			return (!front_.empty()) ? *prev(end(front_)) : *begin(back_);
		}


		T & Back() {
			return (!back_.empty()) ? *prev(end(back_)) : *begin(front_);
		}

		const T & Back() const {
			return (!back_.empty()) ? *prev(end(back_)) : *begin(front_);
		}

		void PushFront(const T value) {
			front_.push_back(value);
		}

		void PushBack(const T value) {
			back_.push_back(value);
		}

	private:
		vector<T> front_;
		vector<T> back_;
	};

	void TestDeque();

#pragma endregion

	// Range class based on pair of iterators
	template<class It>
	class ItRange
	{
	private:
		It first, last;
	public:
		ItRange(It f, It l) : first(f), last(l) {}

		It begin() const { return first; }
		It end() const { return last; }

		size_t size() const { return distance(first, last); }

		bool operator==(const ItRange & other) {
			return tie(first, last) == tie(other.first, other.last);
		}

		bool operator!=(const ItRange & other) {
			return !(*this == other);
		}
	};

	template<class It>
	ItRange<It> MakeRange(It first, It second)
	{
		return{ first, second };
	}

	template<class Container>
	auto Head(Container & body, size_t size)
	{
		return MakeRange(begin(body), next(begin(body), min(size, body.size())));
	}

#pragma region Paginator

	template<class It>
	class Paginator
	{
	public:
		Paginator(It beg, It end, size_t page_size)
		{
			size_t pages_count = static_cast<size_t> (ceil((double)distance(beg, end) / page_size));
			pages_.reserve(pages_count);

			for (size_t page_id = 0; page_id < pages_count; ++page_id)
			{
				It start = beg + page_id * page_size;
				size_t cur_page_size = (distance<It>(start, end) < page_size) ? distance<It>(start, end) : page_size;

				pages_.push_back(MakeRange(start, start + cur_page_size));
			}
		}

		auto begin() {
			return pages_.begin();
		}

		auto end() {
			return pages_.end();
		}

		size_t size() const {
			return pages_.size();
		}

	private:
		vector<ItRange<It>> pages_;
	};

	template<class It>
	Paginator<It> MakePaginator(It beg, It end, size_t page_size)
	{
		return Paginator<It>(beg, end, page_size);
	}

	template <typename Container>
	auto  Paginate(Container & c, size_t page_size)
	{
		return MakePaginator(c.begin(), c.end(), page_size);
	}

#pragma endregion

#pragma region Плохой Макрос

#define PRINT_VALUES(out, x, y) out << (x) << endl << (y) << endl

#pragma endregion

#pragma region Макрос UNIQ_ID

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID_CREATE(lineno) UNIQ_ID_IMPL(lineno)
#define UNIQ_ID UNIQ_ID_CREATE(__LINE__)

#pragma endregion


}

