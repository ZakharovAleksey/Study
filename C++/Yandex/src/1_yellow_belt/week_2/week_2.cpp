#include "..\..\2_red_belt\week_2\week_2.h"
#include"..\..\unit_test\unit_test.h"

#include<fstream>

void week_2::TestHotelManager()
{
	// !!! Failed on 4 test !!!

	// Yandex course tests
	{
		week_2::HotelManager hm(11);
		ASSERT_EQUAL(hm.Clients("Marriott"), 0);
		ASSERT_EQUAL(hm.Rooms("Marriott"), 0);
		hm.Book(10, "FourSeasons", 1, 2);
		hm.Book(10, "Marriott", 1, 1);
		hm.Book(86409, "FourSeasons", 2, 1);
		ASSERT_EQUAL(hm.Clients("FourSeasons"), 2);
		ASSERT_EQUAL(hm.Rooms("FourSeasons"), 3);
		ASSERT_EQUAL(hm.Clients("Marriott"), 1);
		hm.Book(86410, "Marriott", 2, 10);
		ASSERT_EQUAL(hm.Rooms("FourSeasons"), 1);
		ASSERT_EQUAL(hm.Rooms("Marriott"), 10);
	}
	// Additional tests
	{
		HotelManager manager(22);

		// Test empty bookings
		ASSERT_EQUAL(manager.Clients("mariott"), 0);
		ASSERT_EQUAL(manager.Rooms("mariott"), 0);

		manager.Book(0, "mariott", 1, 10);
		manager.Book(0, "mariott", 2, 1);
		manager.Book(0, "mariott", 3, 1);
		manager.Book(0, "mariott", 4, 1);
		manager.Book(0, "hilton", 1, 1);
		manager.Book(1, "hilton", 2, 1);
		// Add booking attempt with no rooms
		manager.Book(1, "lol", 100, 0);

		// Test correctness
		ASSERT_EQUAL(manager.Clients("mariott"), 4);
		ASSERT_EQUAL(manager.Rooms("mariott"), 13);
		ASSERT_EQUAL(manager.Clients("hilton"), 2);
		ASSERT_EQUAL(manager.Rooms("hilton"), 2);
		ASSERT_EQUAL(manager.Clients("lol"), 0);
		ASSERT_EQUAL(manager.Rooms("lol"), 0);

		// Test event past 1 day resets statics
		manager.Book(86400, "mariott", 1, 1);
		ASSERT_EQUAL(manager.Clients("mariott"), 1);
		ASSERT_EQUAL(manager.Rooms("mariott"), 1);
		ASSERT_EQUAL(manager.Clients("hilton"), 1);
		ASSERT_EQUAL(manager.Rooms("hilton"), 1);

		// Test no clients and room for the last day
		manager.Book(86401, "mariott", 5, 1);
		ASSERT_EQUAL(manager.Clients("mariott"), 2);
		ASSERT_EQUAL(manager.Rooms("mariott"), 2);
		ASSERT_EQUAL(manager.Clients("hilton"), 0);
		ASSERT_EQUAL(manager.Rooms("hilton"), 0);
	}
}

void week_2::MainHotelManagerFunc() {
	ifstream ifs; ifs.open("lol.txt");

	size_t q = 0; ifs >> q;
	HotelManager hm(q);

	string command, hotel_name;
	uint32_t time;
	size_t client_id, room_count;

	for (size_t i = 0; i < q; ++i)
	{
		ifs >> command;
		if (command[0] == 'B') {
			ifs >> time >> hotel_name >> client_id >> room_count;
			hm.Book(time, hotel_name, client_id, room_count);
		}
		else if (command[0] == 'C') {
			ifs >> hotel_name;
			cout << hm.Clients(hotel_name) << endl;
		}
		else {
			ifs >> hotel_name;
			cout << hm.Rooms(hotel_name) << endl;
		}
	}

	ifs.close();
}
