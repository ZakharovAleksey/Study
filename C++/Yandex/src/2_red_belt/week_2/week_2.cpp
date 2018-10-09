#include"week_2.h"
#include"../../unit_test/unit_test.h"


namespace week_2 {
	
	using namespace unit_test;

	void HM::Book(const string & hotel_name, int64_t cur_time, int32_t client_id, uint16_t room_count) {
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

		auto it = clients_numb_.find(hotel_name);
		if (it == end(clients_numb_)) {
			clients_numb_[hotel_name][client_id] = 1u;
		}
		else {
			++it->second[client_id];
		}

		rooms_numb_[hotel_name] += static_cast<uint32_t>(room_count);

		int64_t last_time = cur_time - DAY_TIME;
		while (book_queries_[start_id].cur_time <= last_time) {
			//clients_numb_[book_queries_[start_id].hotel_name].erase(book_queries_[start_id].client_id);
			--clients_numb_[book_queries_[start_id].hotel_name][book_queries_[start_id].client_id];

			if (clients_numb_[book_queries_[start_id].hotel_name][book_queries_[start_id].client_id] == 0u) {
				clients_numb_[book_queries_[start_id].hotel_name].erase(book_queries_[start_id].client_id);
			}



			rooms_numb_[book_queries_[start_id].hotel_name] -=
				static_cast<uint32_t>(book_queries_[start_id].room_count);

			++start_id;
		}
	}

	uint32_t HM::Clients(const string & hotel_name) const {
		auto it = clients_numb_.find(hotel_name);

		if (it == end(clients_numb_) || clients_numb_.empty()) {
			return 0u;
		}

		return it->second.size();
	}

	uint32_t HM::Rooms(const string & hotel_name) const {
		auto it = rooms_numb_.find(hotel_name);
		if (it == end(rooms_numb_) || rooms_numb_.empty()) {
			return 0u;
		}

		return it->second;
	}

	void TestHMOneUserReserve() {
		/*
		Check that is one client reserved several rooms in the same hotel,
		algorithm will told us, that that onlly ONE client has reserved
		this hotel
		*/

		HM hm(10);

		hm.Book("Azimut", 10, 1, 10);
		AssertEqual(hm.Clients("Azimut"), 1u, "Azimut clients 0");
		AssertEqual(hm.Rooms("Azimut"), 10u, "Azimut rooms 0");

		hm.Book("Azimut", 10, 1, 10);
		AssertEqual(hm.Clients("Azimut"), 1u, "Azimut clients 1");
		AssertEqual(hm.Rooms("Azimut"), 20u, "Azimut rooms 1");

		hm.Book("Azimut", 20, 2, 10);
		hm.Book("Azimut", 50, 1, 10);

		AssertEqual(hm.Clients("Azimut"), 2u, "Azimut clients 2");
		AssertEqual(hm.Rooms("Azimut"), 40u, "Azimut rooms 2");
	}

	void TestHMTime() {
		HM hm(10);

		hm.Book("First", 10, 1, 10);
		hm.Book("Second", 10, 1, 10);

		hm.Book("First", 20, 1, 10);
		hm.Book("Second", 30, 2, 10);

		AssertEqual(hm.Clients("First"), 1u, "First clients 0");
		AssertEqual(hm.Clients("Second"), 2u, "Second clients 0");

		AssertEqual(hm.Rooms("First"), 20u, "First rooms 0");
		AssertEqual(hm.Rooms("Second"), 20u, "Second rooms 0");


		hm.Book("Third", 10 + DAY_TIME, 1, 10);

		AssertEqual(hm.Clients("First"), 1u, "First clients 1");
		AssertEqual(hm.Clients("Second"), 1u, "Second clients 1");
		AssertEqual(hm.Clients("Third"), 1u, "Second clients 1");

		AssertEqual(hm.Rooms("First"), 10u, "First rooms 1");
		AssertEqual(hm.Rooms("Second"), 10u, "Second rooms 1");
		AssertEqual(hm.Rooms("Third"), 10u, "First rooms 1");

		hm.Book("Third", 20 + DAY_TIME, 2, 10);

		AssertEqual(hm.Clients("First"), 0u, "First clients 2");
		AssertEqual(hm.Clients("Second"), 1u, "Second clients 2");
		AssertEqual(hm.Clients("Third"), 2u, "Second clients 2");

		AssertEqual(hm.Rooms("First"), 0u, "First rooms 2");
		AssertEqual(hm.Rooms("Second"), 10u, "Second rooms 2");
		AssertEqual(hm.Rooms("Third"), 20u, "First rooms 2");

		hm.Book("Third", 30 + DAY_TIME, 3, 10);

		AssertEqual(hm.Clients("First"), 0u, "First clients 3");
		AssertEqual(hm.Clients("Second"), 0u, "Second clients 3");
		AssertEqual(hm.Clients("Third"), 3u, "Second clients 3");

		AssertEqual(hm.Rooms("First"), 0u, "First rooms 3");
		AssertEqual(hm.Rooms("Second"), 0u, "Second rooms 3");
		AssertEqual(hm.Rooms("Third"), 30u, "First rooms 3");


		hm.Book("Forth", 3u * DAY_TIME, 3, 10);

		AssertEqual(hm.Clients("First"), 0u, "First clients 4");
		AssertEqual(hm.Clients("Second"), 0u, "Second clients 4");
		AssertEqual(hm.Clients("Third"), 0u, "Second clients 4");

		AssertEqual(hm.Rooms("First"), 0u, "First rooms 4");
		AssertEqual(hm.Rooms("Second"), 0u, "Second rooms 4");
		AssertEqual(hm.Rooms("Third"), 0u, "First rooms 4");
	}
}