#ifndef BIN_SEARCH_TREE_H
#define BIN_SEARCH_TREE_H

#pragma once

#include<iostream>
#include<memory>
#include<fstream>

namespace tree
{
	using namespace std;

	struct NodeBST;
	using NodeBSTPtr = shared_ptr<NodeBST>;

	struct NodeBST
	{
		int value;

		NodeBSTPtr left;
		NodeBSTPtr right;
		NodeBSTPtr parent;

		NodeBST(int value) : value(value), left(nullptr), right(nullptr), parent(nullptr) {}

		friend ostream & operator<<(ostream & os, const NodeBST & node);
	};


	// BST - Binary Search Tree with no selfbalancing
	// 
	class BSTree
	{
	public:
		BSTree();
		BSTree(NodeBSTPtr root) : root_(root) {}

		void Insert(const int & value);
		bool Find(const int & value);
		void Remove(const int & value);
		bool IsEmpty() const;

		// Calculates the depth of the tree
		size_t GetDepth() const;

		friend ostream & operator<<(ostream & os, const BSTree & st);

	private:

		// Help functions
		void InorderTraversal(NodeBSTPtr cur_node, ostream & os = cout) const;
		size_t GetDepth(NodeBSTPtr cur_node) const;

		NodeBSTPtr Find(NodeBSTPtr cur_node, const int & value) const;

		void Remove(NodeBSTPtr cur_node);
		void RemoveLeaf(NodeBSTPtr cur_node);
		void RemoveOneChild(NodeBSTPtr cur_node);

		// Finds the node with highest value that is less than cur_node value
		NodeBSTPtr Predecessor(NodeBSTPtr cur_node);

	private:
		NodeBSTPtr root_;
	};

	void BSTreeTest();
}


#endif // !BIN_SEARCH_TREE_H