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
	using namespace std;

#pragma region —ловарь

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

#pragma endregion

#pragma region Ёкспрессы

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

#pragma region Ёлектронна€ книга

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

	// TODO

#pragma region —истема бронировани€ отелей

	class HotelManager
	{
	public:

		struct Order
		{
			int64_t time;
			uint32_t client_id;
			uint16_t room_count;
			string hotel_name;

			Order(int64_t time, uint32_t client_id, uint16_t room_count, string hotel_name) : 
				time(time), 
				client_id(client_id), 
				room_count(room_count), 
				hotel_name(hotel_name) {}
		};

		HotelManager(size_t Q) : current_time_(0), order_id(0u), start_id(0u) {
			orders_.reserve(Q);
			reserved_clients_.reserve(Q);
			reserved_rooms_.reserve(Q);
		}

		void Book(int64_t time, const string & hotel_name, uint32_t client_id, uint16_t room_count) 
		{
			if (room_count == 0) {
				return;
			}

			current_time_ = time;
			orders_.push_back({ time, client_id, room_count, hotel_name });

			RemoveOld();
			
			// Add client id
			auto it = reserved_clients_.find(hotel_name);
			if (it == reserved_clients_.end())
				reserved_clients_.insert({ hotel_name, {client_id} });
			else
				it->second.insert(client_id);

			// Add room count
			auto room_it = reserved_rooms_.find(hotel_name);
			if (room_it == reserved_rooms_.end())
				reserved_rooms_.insert({ hotel_name, room_count });
			else
				room_it->second += static_cast<uint64_t>(room_count);

		}

		size_t Clients(const string hotel_name)
		{
			auto it = reserved_clients_.find(hotel_name);

			return (it == end(reserved_clients_)) ? 0 : it->second.size();
		}

		uint64_t Rooms(const string hotel_name)
		{
			auto it = reserved_rooms_.find(hotel_name);

			return (it == end(reserved_rooms_)) ? 0 : it->second;
		}

	private:
		void RemoveOld()
		{
			size_t new_start_id = start_id;
			while (orders_[new_start_id].time <= current_time_ - DAY_TIME)
				++new_start_id;

			for (size_t i = start_id; i != new_start_id; ++i)
			{
				reserved_clients_[orders_[i].hotel_name].erase(orders_[i].client_id);
				reserved_rooms_[orders_[i].hotel_name] -= static_cast<uint64_t>(orders_[i].room_count);
			}

			start_id = new_start_id;
		}

	private:
		static const int MAX_COUNT = 100'000;
		static const int64_t DAY_TIME = 86'400;

		int64_t current_time_;
		size_t order_id;
		size_t start_id;

		vector<Order> orders_;	// id of operation ond order
		unordered_map<string, set<uint32_t>> reserved_clients_;	// »м€ отле€ и число различных клиентов бронировавших номера в этом отеле
		unordered_map<string, uint64_t> reserved_rooms_;		// „исло номерв забронированном в этом отеле за полсдение сутки
	};

	void TestHotelManager();

	void MainHotelManagerFunc();

#pragma endregion

}