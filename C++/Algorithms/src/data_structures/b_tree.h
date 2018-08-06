#ifndef B_TREE_H
#define B_TREE_H

#pragma once

#include<vector>
#include<set>
#include<unordered_map>
#include<memory>
#include<fstream>

namespace tree
{
	using namespace std;

	struct BTreeNode;
	using BTreeNodePtr = shared_ptr<BTreeNode>;

	struct BTreeNode
	{
		size_t el_numb;
		size_t min_degree;
		bool is_leaf;

		// Stores separate keys of current node
		vector<int> keys;
		// Stores pointers to nodes for appropriate separate keys
		vector<BTreeNodePtr> body;

		BTreeNode(size_t _min_degree, bool _is_leaf);

		BTreeNodePtr Search(const int & key);

		void InsertToNonFullNode(const int & key);
		void SplitNode(int i, BTreeNodePtr cur_node);

		friend ostream & operator<<(ostream & os, const BTreeNode & node);
	};

	// BTree implementation [only Insert() because Delete() is terrible]
	// [NOT B+ Tree : so keys are stored not only in leafs but through all tree body]
	// https://www.geeksforgeeks.org/b-tree-set-1-introduction-2/
	class BTree
	{
	public:
		BTree(size_t min_degree);

		bool Find(const int & value);
		void Insert(const int & value);

		friend ostream & operator<<(ostream & os, const BTree & bt);

	private:

		BTreeNodePtr Search(const int & value);
	private:
		BTreeNodePtr root_;
		size_t min_degree_;
	};


	void BTreeTest();
}


#endif // !B_TREE_H
