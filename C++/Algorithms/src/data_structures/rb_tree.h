#ifndef RB_TREE_H
#define RB_TREE_H

#pragma once

#include<memory>
#include<fstream>


namespace tree
{
	using namespace std;

	struct RBNode;
	using RBNodePtr = shared_ptr<RBNode>;

	enum class Color
	{
		RED, BLACK
	};

	struct RBNode
	{
		Color c;
		RBNodePtr left, right, parent;
		int value;

		RBNode(int _value) : c(Color::RED), left(nullptr), right(nullptr), value(_value) {}

		friend ostream & operator<<(ostream & os, const RBNode & n);
	};

	// Red-Black Tree
	// https://www.geeksforgeeks.org/c-program-red-black-tree-insertion/
	// Visualisation : https://www.cs.usfca.edu/~galles/visualization/RedBlack.html
	class RBTree
	{
	public:
		RBTree();
		
		void Insert(const int & value);
		bool Find(const int & value);
		RBNodePtr Search(const int & value);

		size_t MaxDepth() const;

		friend ostream & operator<<(ostream & os, const RBTree & rbt);

	private:

		RBNodePtr InsertBST(const int & value);
		// Make tree balanced via ReColoring and Rotation procedures
		void FixBalance(RBNodePtr cur_node);

		// All Rotation on ReColoring cases
		RBNodePtr LeftRedUncle(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node);
		RBNodePtr RightRedUncle(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node);

		RBNodePtr LeftLeftCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node);
		RBNodePtr RightRightCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node);

		RBNodePtr LeftRightCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node);
		RBNodePtr RightLeftCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node);

		void RotateRight(RBNodePtr par_node);
		void RotateLeft(RBNodePtr par_node);

		RBNodePtr SearchHelper(RBNodePtr cur_node, const int & value);

		size_t MaxDepthHelper(RBNodePtr cur_node) const;

		void InorderTraverse(RBNodePtr cur_node, ostream & os) const;
	private:

		RBNodePtr root_;
	};

	void RBTreeTest();

}

#endif // !RB_TREE_H
