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


int main() 
{
	//TestAll();

	/*
	TestGraph();
	
	graph::Graph gr(true);
	gr.Add(2, 1);
	gr.Add(2, 8);
	gr.Add(1, 7);
	gr.Add(3, 1);
	gr.Add(3, 8);
	gr.Add(8, 0);
	gr.Add(0, 3);
	gr.Add(7, 3);
	gr.Add(3, 4);
	gr.Add(7, 4);
	gr.Add(0, 5);
	gr.Add(4, 6);
	gr.Add(6, 5);
	gr.Add(5, 9);
	gr.Add(9, 6);

	cout << gr;
	cout << "v numb = " << gr.GetVertexNumb() << endl;

	auto lol = gr.StrongConnectedCompNumb();

	for (auto i : lol)
	{
		cout << i.first << ": ";
		copy(begin(i.second), end(i.second), ostream_iterator<int>(cout, " "));
		cout << endl;
	}

	gr.BFS(3);

	auto shp = gr.ShortestDistance(4, 7);
	copy(begin(shp), end(shp), ostream_iterator<size_t>(cout, " ->"));
	cout << endl;*/

	/*cout << "v numb = " << gr.GetVertexNumb() << endl;
	gr.DFS();

	auto res = gr.ConnectedCompNumb();
	for (const auto & i : res)
	{
		cout << i.first << ": ";
		copy(begin(i.second), end(i.second), ostream_iterator<int>(cout, " "));
		cout << endl;
	}
	
	cout << gr.IsCircleDetect() << endl;
	

	cout << "----\n";
	gr.TopoligicalSort();*/
		

	graph::GraphWeight wg;
	wg.Add(0, 1, 4);
	wg.Add(0, 2, 2);
	wg.Add(1, 2, 3);
	wg.Add(2, 1, 1);
	wg.Add(1, 3, 2);
	wg.Add(1, 4, 3);
	wg.Add(2, 3, 4);
	wg.Add(2, 4, 5);
	wg.Add(3, 3, 0);
	wg.Add(4, 3, 1);


	cout << wg;

	auto path = wg.ChipestPath(0, 4);
	cout << path;
	cout << wg.ChipestPathCost(0, 4) << endl;


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
	//tr.RunTest(data_struct::BinTreeTest, "BinTreeTest");
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
}