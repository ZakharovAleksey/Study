#pragma once

#include <map>
#include <string>
#include <algorithm>
#include <string>
#include <vector>
#include<set>
#include<numeric>

#include<unordered_map>
#include<cmath>
#include<deque>

namespace week_2
{
#pragma once

	using namespace std;

	class Learner {
	private:
		set<string> dict;

	public:
		int Learn(const vector<string>& words) {
			set<string> tmp(begin(words), end(words));
			size_t uniq_count = 0;
			for (const auto word : tmp)
				if (dict.find(word) == dict.end())
				{
					dict.insert(word);
					++uniq_count;
				}

			return uniq_count;
		}

		vector<string> KnownWords() {
			return vector<string>(begin(dict), end(dict));
		}
	};


#pragma region Экспрессы

	class TrainScheduler
	{
	public:

		void Add(int start, int finish) {
			AddHelper(start, finish);
			AddHelper(finish, start);
		}

		int Go(int start, int finish){
			if (start == finish)
				return 0;

			int result = Distance(start, finish);

			auto start_it = body_.find(start);

			// If there is no station to start -> go [finish - start]
			// or the station do not live in any diraction
			if (start_it == body_.end() || start_it->second.empty())
				return result;
			
			// If less than mininum possible station
			if (finish < *begin(start_it->second))
				return min(result, Distance(finish, *begin(start_it->second)));

			// If higer than maximum possible station
			if (finish > *rbegin(start_it->second))
				return min(result, Distance(finish, *rbegin(start_it->second)));

			// Now it is inside the range [min, max] and size is bigger than 1
			// Because it is siuted in previous cases or next one when they are equal
			auto next_ = start_it->second.lower_bound(finish);
			if (*next_ == finish)
				return 0;

			auto prev_ = prev(next_);
			return min(result, min(Distance(*prev_, finish), Distance(*next_, finish)));
		}

	private:
		void AddHelper(int start, int finish)
		{
			if (body_.find(start) == body_.end())
				body_.insert({ start,{ finish } });
			else
				body_[start].insert(finish);
		}

		int Distance(int start, int finish)
		{
			if (start * finish > 0)
				return abs(start - finish);

			return max(start, finish) - min(start, finish);
		}

	private:
		unordered_map<int, set<int> > body_;
	};

#pragma endregion


#pragma region Электронная книга

	class BookManager
	{
	public:

		BookManager() : tmp_ids_(MAX_USER_COUNT_ + 1, -1), pages_count_(MAX_PAGES_COUNT + 1, 0) {}

		void Read(int user_id, int page)
		{
			auto it = user_ids_.find(user_id);
			
			if (it == user_ids_.end())
			{
				++pages_count_[page];

				tmp_ids_[user_id] = page;
				user_ids_.insert({ user_id, page });
			}
			else
			{
				auto old_page = it->second;
				--pages_count_[old_page];
				++pages_count_[page];

				tmp_ids_[user_id] = page;
				user_ids_[user_id] = page;
			}
		}

		double Cheer(int user_id)
		{
			if (tmp_ids_[user_id] == -1)
				return 0.;

			if (user_ids_.size() == 1)
				return 1.;

			return (double) accumulate(begin(pages_count_), begin(pages_count_) + tmp_ids_[user_id], 0.) / 
				((double)user_ids_.size() - 1.);
		}

	private:
		static const int MAX_USER_COUNT_ = 100'000;
		static const int MAX_PAGES_COUNT = 1'000;

		vector<int> tmp_ids_;
		map<int, int> user_ids_;
		multiset<int> readed_pages_;

		vector<int> pages_count_;
	};

#pragma endregion


#pragma region Система бронирования отелей

	class HotelManager
	{
	public:
		HotelManager() : current_time_(0) {}

		void Book(int32_t time, const string & hotel_name, size_t client_id, size_t room_count) 
		{
			current_time_ = time;
		}

		int Clients(const string hotel_name)
		{
			
		}

		int Rooms(const string hotel_name)
		{
			
		}

	private:
		static const int MAX_CLIENT_COUNT = 1'000'000'000;
		static const int32_t DAY_TIME = 86'400;

		int32_t current_time_;
		deque<int> a;
		unordered_map<string, size_t> reserved_clients_;	// Имя отлея и число различных клиентов бронировавших номера в этом отеле
		unordered_map<string, size_t> reserved_rooms_;		// Число номерв забронированном в этом отеле за полсдение сутки
	};

#pragma endregion


}