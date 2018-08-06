#include"unit_test\unit_test.h"
#include"sorting\sorting.h"
#include"data_structures\data_structures.h"
#include"data_structures\bloom_filter.h"
#include"practice\practice.h"

#include"data_structures\splay_tree.h"
#include"data_structures\bin_search_tree.h"
#include"data_structures\prefix_tree.h"
#include"data_structures\b_tree.h"

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
	TestAll();


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

	tr.RunTest(tree::TrieTreeTest, "TrieTreeTest");

	tr.RunTest(tree::BTreeTest, "BTreeTest");
}

//#include<iostream>
//#include<cstring>
//
//using namespace std;
//
//const int ALPHABET_SIZE = 26;
//
//// trie node
//struct TrieNode
//{
//	struct TrieNode *children[ALPHABET_SIZE];
//
//	// isEndOfWord is true if the node represents
//	// end of a word
//	bool isEndOfWord;
//};
//
//// Returns new trie node (initialized to NULLs)
//struct TrieNode *getNode(void)
//{
//	struct TrieNode *pNode = new TrieNode;
//
//	pNode->isEndOfWord = false;
//
//	for (int i = 0; i < ALPHABET_SIZE; i++)
//		pNode->children[i] = NULL;
//
//	return pNode;
//}
//
//// If not present, Inserts key into trie
//// If the key is prefix of trie node, just
//// marks leaf node
//void Insert(struct TrieNode *root, string key)
//{
//	struct TrieNode *pCrawl = root;
//
//	for (int i = 0; i < key.length(); i++)
//	{
//		int index = key[i] - 'a';
//		if (!pCrawl->children[index])
//			pCrawl->children[index] = getNode();
//
//		pCrawl = pCrawl->children[index];
//	}
//
//	// mark last node as leaf
//	pCrawl->isEndOfWord = true;
//}
//
//// Returns true if key presents in trie, else
//// false
//bool search(struct TrieNode *root, string key)
//{
//	struct TrieNode *pCrawl = root;
//
//	for (int i = 0; i < key.length(); i++)
//	{
//		cout << key[i] << " ( " << pCrawl->isEndOfWord << " ) ";
//		int index = key[i] - 'a';
//		if (!pCrawl->children[index])
//			return false;
//
//		pCrawl = pCrawl->children[index];
//	}
//	cout << endl;
//	return (pCrawl != NULL && pCrawl->isEndOfWord);
//}
//
//// Driver
//int main()
//{
//	// Input keys (use only 'a' through 'z'
//	// and lower case)
//	string keys[] = {"by","bye"};
//	int n = sizeof(keys) / sizeof(keys[0]);
//
//	struct TrieNode *root = getNode();
//
//	// Construct trie
//	for (int i = 0; i < n; i++)
//		Insert(root, keys[i]);
//
//	// Search for different keys
//	search(root, "by");
//	search(root, "bye");
//	return 0;
//}