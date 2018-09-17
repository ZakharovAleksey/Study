#include"unit_test\unit_test.h"
#include"sorting\sorting.h"
#include"data_structures\data_structures.h"
#include"data_structures\bloom_filter.h"
#include"practice\practice.h"

#include"data_structures\splay_tree.h"
#include"data_structures\bin_search_tree.h"
#include"data_structures\prefix_tree.h"
#include"data_structures\b_tree.h"
#include"data_structures\rb_tree.h"

#include"data_structures/heaps.h"
#include"data_structures\cartesian_tree.h"

// Graph
#include"graphs\graph.h"

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

void MegreSortNonReq(vector<int> & v)
{
	for (size_t i = 1; i < static_cast<size_t>(log2(v.size())) + 1; ++i)
	{
		auto cur_it = begin(v); size_t cur_step = (size_t)pow(2, i);
		while (cur_it != end(v))
		{
			inplace_merge ( cur_it, cur_it + cur_step / 2, cur_it + cur_step );
			cur_it += cur_step;
		}
	}
}

template<class T>
void RadixSort(vector<T> & v) {
	size_t max = static_cast<size_t> (*max_element(begin(v), end(v)));

	for (size_t i = 1; max / i > 0; i *= 10) {
		CountSortHelper(v, int(i));
		cout << i << endl;
	}
}

int main() 
{
	vector<int> v = { 15, 6, 63, 12, 7, 8, 90, 51, 25, 15};
	RadixSort(v);
	cout << v << endl;
	//MegreSortNonReq(v);
	//TestAll();

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
	tr.RunTest(data_struct::DequeTest, "DequeTest");

	tr.RunTest(tree::SplayTreeTest, "SplayTreeTest");
	tr.RunTest(tree::BSTreeTest, "BSTreeTest");
	tr.RunTest(tree::TrieTreeTest, "TrieTreeTest");
	tr.RunTest(tree::BTreeTest, "BTreeTest");
	tr.RunTest(tree::RBTreeTest, "RBTreeTest");

	// Heaps
	tr.RunTest(heap::BinHeapTest, "BinHeapTest");
	tr.RunTest(heap::BinomHeapTest, "BinomHeapTest");
	tr.RunTest(tree::TestCartesianTree, "TestCartesianTree");

	// Graphs
	tr.RunTest(graph::TestWeightGraph, "TestWeightGraph");
	tr.RunTest(graph::TestGraph, "TestGraph");
}