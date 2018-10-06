#include"unit_test\unit_test.h"
#include"time_logger\time_logger.h"

using namespace std;
using namespace unit_test;
using namespace log_time;

//--------------------------------------------------------------





#include <algorithm>
#include<numeric>
#include <array>
#include <iostream>
#include <random>
#include <vector>
#include<unordered_map>

using namespace std;

#include<set>

static const uint8_t	MAX_HOTEL_LEN = 13u;
static const uint32_t	MAX_QUERY_COUNT = 10'000u;
static const uint64_t	DAY_TIME = 86'400u;

struct BookData {
	int64_t cur_time;
	uint32_t client_id;
	uint16_t room_count;

	string hotel_name;

	BookData() : cur_time(UINTMAX_MAX), client_id(0u), room_count(0u), hotel_name(string()) {}

};

class HM {
public:
	HM(uint32_t query_count) : start_id(0u), cur_id(0u) {
		clients_numb_.reserve(query_count);
		rooms_numb_.reserve(query_count);
	}

	void Book(const string & hotel_name, int64_t cur_time = 0u, int32_t client_id = 0u, uint16_t room_count = 0u) {
		// If client do not reserve any room -> do nothing
		if (room_count == 0) {
			return;
		}
		else {
			book_queries_[cur_id].cur_time = cur_time;
			book_queries_[cur_id].client_id = client_id;
			book_queries_[cur_id].room_count = room_count;
			book_queries_[cur_id].hotel_name = hotel_name;
			
			++cur_id;
		}
		
		clients_numb_[hotel_name].insert(client_id);
		rooms_numb_[hotel_name] += static_cast<uint32_t>(room_count);

		int64_t last_time = cur_time - DAY_TIME;
		while (book_queries_[start_id].cur_time <= last_time) {
			clients_numb_[book_queries_[start_id].hotel_name].erase(book_queries_[start_id].client_id);

			rooms_numb_[book_queries_[start_id].hotel_name] -= 
				static_cast<uint32_t>(book_queries_[start_id].room_count);

			++start_id;
		}
	}

	uint32_t Clients(const string & hotel_name) const {
		auto it = clients_numb_.find(hotel_name);

		if (it == end(clients_numb_) || clients_numb_.empty()) {
			return 0u;
		}
		
		return it->second.size();
	}

	uint32_t Rooms(const string & hotel_name) const {
		auto it = rooms_numb_.find(hotel_name);
		if (it == end(rooms_numb_) || rooms_numb_.empty()) {
			return 0u;
		}
		
		return it->second;
	}

private:
	size_t start_id;
	size_t cur_id;

	array<BookData, MAX_QUERY_COUNT> book_queries_;
	// Необходимо сделать set<pair<uint32_t, size_t>>: где size_t - это количество бронирований этого отеля пользователем client_id
	// и поэтому надо при удалении декремнтировать второй индекс и если он равен 0, то тогда удалять номер этого пользователя:
	// Как пример можно посмотреть третий тест (он не проходит)
	unordered_map<string, set<uint32_t>> clients_numb_; 
	unordered_map<string, uint32_t> rooms_numb_;
};

void TestHMYandex() {
	HM hm(11u);
	AssertEqual(hm.Clients("Marriott"), 0u, "");
	AssertEqual(hm.Rooms("Marriott"),	0u, "");
	
	hm.Book("FourSeasons", 10, 1, 2);
	hm.Book("Marriott", 10, 1, 1);
	hm.Book("FourSeasons", 86409, 2, 1);

	AssertEqual(hm.Clients("FourSeasons"), 2u, "");
	AssertEqual(hm.Rooms("FourSeasons"), 3u, "");
	AssertEqual(hm.Clients("Marriott"), 1u, "");

	hm.Book("Marriott", 86410, 2, 10);
	AssertEqual(hm.Rooms("FourSeasons"), 1u, "");
	AssertEqual(hm.Rooms("Marriott"), 10u, "");
}

void TestHMOneUserReserve() {
	/*
		Check that is one client reserved several rooms in the same hotel,
		algorithm will told us, that that onlly ONE client has reserved 
		this hotel
	*/

	HM hm(10);
	
	hm.Book("Azimut", 10, 1, 10);
	AssertEqual(hm.Clients("Azimut"),	1u,		"Azimut clients 0");
	AssertEqual(hm.Rooms("Azimut"),		10u,	"Azimut rooms 0");

	hm.Book("Azimut", 10, 1, 10);
	AssertEqual(hm.Clients("Azimut"),	1u,		"Azimut clients 1");
	AssertEqual(hm.Rooms("Azimut"),		20u,	"Azimut rooms 1");

	hm.Book("Azimut", 20, 2, 10);
	hm.Book("Azimut", 50, 1, 10);

	AssertEqual(hm.Clients("Azimut"),	2u,		"Azimut clients 2");
	AssertEqual(hm.Rooms("Azimut"),		40u,	"Azimut rooms 2");
}

void TestHMTime() {
	HM hm(10);

	hm.Book("First", 10, 1, 10);
	hm.Book("Second", 10, 1, 10);

	hm.Book("First", 20, 1, 10);
	hm.Book("Second", 30, 2, 10);
	
	AssertEqual(hm.Clients("First"), 1u, "First clients 0");
	AssertEqual(hm.Clients("Second"), 2u, "Second clients 0");

	AssertEqual(hm.Rooms("First"), 20u, "First rooms 1");
	AssertEqual(hm.Rooms("Second"), 20u, "Second rooms 1");


	hm.Book("Third", 10 + DAY_TIME, 1, 10);

	AssertEqual(hm.Clients("First"),	1u, "First clients 0");
	AssertEqual(hm.Clients("Second"),	1u, "Second clients 0");
	AssertEqual(hm.Clients("Third"),	1u, "Second clients 0");

	AssertEqual(hm.Rooms("First"),	10u, "First rooms 1");
	AssertEqual(hm.Rooms("Second"), 10u, "Second rooms 1");
	AssertEqual(hm.Rooms("Third"),	10u, "First rooms 1");


	// Дописать до тех пор, пока все отели не очистятся


}

void Test() {
	TestRunner tr;
	RUN_TEST(tr, TestHMYandex);
	RUN_TEST(tr, TestHMOneUserReserve);
	RUN_TEST(tr, TestHMTime);
}

int main() {
	Test();

	return 0;
}