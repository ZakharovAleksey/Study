#include "data_structures.h"

namespace data_struct
{
	size_t GornerHash(const string & str, size_t a)
	{
		size_t hash = 0;
		for (const auto & symbol : str)
			hash = (hash * a + symbol);
		return hash;
	}

	void HashTableTest()
	{
		HashTable<string> ht;

		const vector<string> names =
		{
			"Zakharov", "Povod", "Gorbushkin", "Izverova",
			"Shagianov", "Novinskaia", "Sadovnicov", "Shmelkin",
			"Ivanov", "Zakharov", "Povod"
		};

		for (const auto & l_name : names)
			ht.AddValue(l_name);


		unit_test::AssertEqual(9u, ht.Size(), "Hash Table Size");
		unit_test::AssertEqual(true, ht.FindValue("Zakharov"), "Hash Table Find Zakharov Init");

		ht.RemoveValue("Zakharov");

		unit_test::AssertEqual(false, ht.FindValue("Zakharov"), "Hash Table Find Zakharov after RemoveValue");

		ht.AddValue("Zakharov");

		unit_test::AssertEqual(true, ht.FindValue("Zakharov"), "Hash Table Find Zakharov after AddValue");
	}

	void BinTreeTest()
	{
		{
			data_struct::BinTree<int> bt;
			vector<int> v = { 0, -1, 6, 3, 2, 1, 4, 5, 0, 3, 2 };

			for (const auto & i : v)
				bt.AddValue(i);

			unit_test::AssertEqual(8, bt.CountValues(), "BinTree Count values");
			unit_test::AssertEqual(true, bt.FindValue(2), "BinTree Find value which is in tree");
			unit_test::AssertEqual(false, bt.FindValue(-2), "BinTree Find value which is in tree");

			bt.RemoveValue(1);
			unit_test::AssertEqual(7, bt.CountValues(), "BinTree Count after Remove a leaf");
			unit_test::AssertEqual(false, bt.FindValue(1), "BinTree Find after Remove a leaf");
			bt.AddValue(1);

			bt.RemoveValue(2);
			unit_test::AssertEqual(7, bt.CountValues(), "BinTree Count after Remove node with ONLY left subtree");
			unit_test::AssertEqual(false, bt.FindValue(2), "BinTree Find after Remove node with ONLY left subtree");
			bt.AddValue(2);

			bt.RemoveValue(3);
			unit_test::AssertEqual(7, bt.CountValues(), "BinTree Count after Remove node with right subtree Case 2 (Recursive)");
			unit_test::AssertEqual(false, bt.FindValue(3), "BinTree Find after Remove node with right subtree Case 2 (Recursive)");

			bt.RemoveValue(4);
			unit_test::AssertEqual(6, bt.CountValues(), "BinTree Count after Remove node with ONLY ONE right node");
			unit_test::AssertEqual(false, bt.FindValue(4), "BinTree Find after Remove node with ONLY ONE right node");
		}
		{
			data_struct::BinTree<int> bt;
			vector<int> v = { 0, -1, 6, 3, 2, 1, 5, 4, 7, 0, 6 };

			for (const auto & i : v)
				bt.AddValue(i);

			bt.RemoveValue(3);
			unit_test::AssertEqual(8, bt.CountValues(), "BinTree Count after Remove node with right subtree Case 1 (NOT Recursive)");
			unit_test::AssertEqual(false, bt.FindValue(3), "BinTree Find after Remove node with right subtree Case 1 (NOT Recursive)");
		}
	}

	void DequeTest()
	{
		data_struct::Deque<int> dq;
		unit_test::AssertEqual(-1, dq.PopFront(), "Deque PopFront from empty");
		unit_test::AssertEqual(-1, dq.PopBack(), "Deque PopBack from empty");

		dq.PushFront(100);
		unit_test::AssertEqual(100, dq.PopFront(), "Deque PopFront from deque with one element (PushFront)");
		dq.PushFront(100);
		unit_test::AssertEqual(100, dq.PopBack(), "Deque PopBack from deque with one element (PushFront)");
		dq.PushBack(100);
		unit_test::AssertEqual(100, dq.PopFront(), "Deque PopFront from deque with one element (PushBack)");
		dq.PushBack(100);
		unit_test::AssertEqual(100, dq.PopBack(), "Deque PopBack from deque with one element (PushBack)");

		const vector<int> v = { 1, 3, 2, 4, 5, 6 };
		for (const auto & i : v)
			dq.PushFront(i);

		unit_test::AssertEqual(6, dq.PopFront(), "Deque PopFront 1");
		unit_test::AssertEqual(5, dq.PopFront(), "Deque PopFront 2");

		for (const auto & i : v)
			dq.PushBack(i);

		unit_test::AssertEqual(6, dq.PopBack(), "Deque PopBack 1");
		unit_test::AssertEqual(5, dq.PopBack(), "Deque PopBack 2");

		ostringstream os; os << dq;
		unit_test::AssertEqual(os.str(), "4 2 3 1 1 3 2 4 \n", "Deque Output");
	}

}