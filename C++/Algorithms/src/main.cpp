#include"unit_test\unit_test.h"
#include"sorting\sorting.h"
#include"data_structures\data_structures.h"
#include"data_structures\bloom_filter.h"
#include"practice\practice.h"

#include"data_structures\splay_tree.h"
#include"data_structures\bin_search_tree.h"
#include"data_structures\prefix_tree.h"


#include<iostream>
#include<vector>
#include<tuple>
#include<iterator>

using namespace std;
using namespace unit_test;
using namespace sort;
using namespace bloom_filter;
using namespace practice;

void TestAll();

int main() 
{
	//TestAll();

	cout << 1 % 10 << endl;

	vector<string> words = { "the", "a", "there", "answer", "any", "by", "bye", "their", "lol" };

	tree::TrieTree t;
	for (const auto & w : words)
		t.Insert(w);
	
	cout << t.Search("any") << endl;
	cout << t.Search("anyl") << endl;
	cout << t.Search("a") << endl;
	cout << t.Search("bye") << endl;
	cout << t.Search("bae") << endl;

	t.Remove("loli");


	return 0;
}

void TestAll()
{
	TestRunner tr;

	// Sorting

	tr.RunTest(sort::TestInsertionSort,			"TestInsertionSort");
	tr.RunTest(sort::TestSelectSort,			"TestSelectSort");
	tr.RunTest(sort::BubbleSortTest,			"BubbleSortTest");
	tr.RunTest(sort::HeapSortTest,				"HeapSortTest");
	tr.RunTest(sort::MergeSortTest,				"MergeSortTest");
	tr.RunTest(sort::QuickSortTest,				"QuickSortTest");
	tr.RunTest(sort::KthOrderStatisticsTest,	"KthOrderStatisticsTest");
	tr.RunTest(sort::CountingSortTest,			"CountingSort");

	// Data Structures

	tr.RunTest(data_struct::HashTableTest, "HashTableTest");
	tr.RunTest(data_struct::BinTreeTest, "BinTreeTest");
	tr.RunTest(data_struct::DequeTest, "DequeTest");

	tr.RunTest(tree::SplayTreeTest, "SplayTreeTest");
	tr.RunTest(tree::BSTreeTest, "BSTreeTest");
}