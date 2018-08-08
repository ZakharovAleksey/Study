#ifndef HEAPS_H
#define HEAPS_H

#pragma once

#include<vector>
#include<list>
#include<memory>

#include<fstream>

namespace heap
{
	using namespace std;

	using VecIt = vector<int>::iterator;
	using ConstVecIt = vector<int>::const_iterator;


	// Binary Heap
	// https://www.geeksforgeeks.org/binary-heap/
	class BinHeap
	{
	public:
		BinHeap();

		VecIt Insert(const int & value);
		int ExtractMin();
		VecIt DecreaseKey(VecIt key_pos, const int & new_value);

		friend ostream & operator<<(ostream & os, const BinHeap & bh);

		VecIt Begin();

	private:
		VecIt SiftUp(VecIt cur_it);
		VecIt SiftDown();

		VecIt GetLeftChild(VecIt cur_it);
		VecIt GetRightChild(VecIt cur_it);
		VecIt GetMin(VecIt left_if, VecIt right_it);

	private:
		vector<int> body_;
	};

	void BinHeapTest();


	// Binomial Heap: naive implementation
	// https://www.geeksforgeeks.org/binomial-heap-2/

	struct BinomTreeNode;
	using  BinomTreeNodePtr = shared_ptr<BinomTreeNode>;

	// Node of Binomial Tree
	struct BinomTreeNode
	{
		// Value stored in current node
		int value;
		// Vector op pointers (vector - because this is tree with different childs sons)
		vector<BinomTreeNodePtr> childs;

		BinomTreeNode(int _value);

		friend ostream & operator<<(ostream & os, const BinomTreeNode & btn);
	};
	
	// Binominal Tree (Nodes of BinomHeap)
	class BinomTree
	{
	public:
		BinomTree(int value);
		BinomTree(BinomTreeNodePtr root, size_t degree);

		// Melt two BinomTrees with the same degree_ field
		void MeltSimilarDegree(const BinomTree & other);
		friend ostream & operator<<(ostream & os, const BinomTree & bt);

		BinomTreeNodePtr root_;
		// Degree of tree
		size_t degree_;
	};

	// Binomial Heap : naive implementation
	class BinomHeap
	{
	public:
		BinomHeap();
		
		void Insert(const int & value);
		int  GetMin() const;
		void ExtractMin();

		friend ostream & operator<<(ostream & os, const BinomHeap & bh);

	private:
		int min_;
		list<BinomTree> bin_trees_;
	};

	void BinomHeapTest();
}

#endif // !HEAPS_H
