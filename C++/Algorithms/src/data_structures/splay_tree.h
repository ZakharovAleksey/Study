#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#pragma once

#include<iostream>
#include<memory>
#include<fstream>

namespace tree
{
	using namespace std;

	struct NodeST;
	using NodeStPtr = shared_ptr<NodeST>;

	struct NodeST
	{
		int value;

		NodeStPtr left;
		NodeStPtr right;
		NodeStPtr parent;

		NodeST(int value) : value(value), left(nullptr), right(nullptr), parent(nullptr) {}

		friend ostream & operator<<(ostream & os, const NodeST & node);
	};


	// Splay Tree - semi balanced and self balanced tree based on rotations
	// https://neerc.ifmo.ru/wiki/index.php?title=Splay-%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE
	class SplayTree
	{
	public:
		SplayTree();

		void Insert(const int & value);
		bool Find(const int & value);
		void Remove(const int & value);
		bool IsEmpty() const;
		int GetRootValue() const;

		// Calculates the depth of the tree
		size_t GetDepth() const;

		friend ostream & operator<<(ostream & os, const SplayTree & st);

	private:

		// Help functions
		void InorderTraversal(NodeStPtr cur_node, ostream & os = cout) const;
		size_t GetDepth(NodeStPtr cur_node) const;

		NodeStPtr Find(NodeStPtr cur_node,  const int & value) const;

		void Remove(NodeStPtr cur_node);
		void RemoveLeaf(NodeStPtr cur_node);
		void RemoveOneChild(NodeStPtr cur_node);

		// Finds the node with highest value that is less than cur_node value
		NodeStPtr Predecessor(NodeStPtr cur_node);

		// Rotations

		// Performs sequence of rotations after which 'cur_node' will be in the root node
		void Splay(NodeStPtr cur_node);
		// Right rotation (BAD implementation) -> think how to do it easily
		NodeStPtr ZigRotation(NodeStPtr & parent_node);
		// Left rotation (BAD implementation) -> think how to do it easily
		NodeStPtr ZagRotation(NodeStPtr & parent_node);

	private:
		NodeStPtr root_;
	};

	void SplayTreeTest();
}


#endif // !SPLAY_TREE_H
