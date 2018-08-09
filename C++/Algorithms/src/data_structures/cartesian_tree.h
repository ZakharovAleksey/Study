#ifndef CARTESIAN_TREE_H
#define CARTESIAN_TREE_H

#include<vector>
#include<memory>
#include<fstream>
#include<tuple>

#include<climits>

namespace tree
{
	using namespace std;

	// Key + Priority pair tmp class
	struct KPPair
	{
		int key, priority;

		KPPair() : key(0), priority(0) {};
		KPPair(int _key, int _priority) : key(_key), priority(_priority) {}

		friend ostream & operator<<(ostream & os, const KPPair & p);
	};

	struct CTNode;
	using CTNodePtr = shared_ptr<CTNode>;

	struct CTNode
	{
		KPPair value;
		CTNodePtr left, right;

		CTNode(KPPair _value) : value(_value), left(nullptr), right(nullptr) {}

		friend ostream & operator<<(ostream & os, const CTNode & node);
	};

	// Treap [CartesianTree, Дуча] (stores pair of values : key + priority)
	// By priority is Heap and in the same way by keys it is BST
	// https://neerc.ifmo.ru/wiki/index.php?title=%D0%94%D0%B5%D0%BA%D0%B0%D1%80%D1%82%D0%BE%D0%B2%D0%BE_%D0%B4%D0%B5%D1%80%D0%B5%D0%B2%D0%BE
	// !!! Hint -> keys could not repeat in this code !!!
	class CarTree
	{
	public:
		CarTree() : root_(nullptr) {}
		CarTree(CTNodePtr new_root) : root_(new_root) {}

		// !!! To make O(n) algorithm we need to stroe pointer to parent !!!
		void BuildTree(vector<KPPair> data);
		void Insert(const KPPair value);
		void Remove(const KPPair value);
		bool Search(const KPPair value);

		friend ostream & operator<<(ostream & os, const CarTree & ct);

	private:
		// Split Treap (appropriate pointers) on two Treaps:
		// !!! First Treap keys < Second Treap keys !!!
		tuple<CTNodePtr, CTNodePtr> Split(CTNodePtr & cur_node, const int & key);

		// Merge Two Treaps
		// !!! First Treap keys MUST BE SMALLER than Second Treap keys !!!
		CTNodePtr Merge(CTNodePtr first, CTNodePtr second);

		CTNodePtr SearchHelper(CTNodePtr cur_node, const KPPair value);

		void InorderTraverse(CTNodePtr cur_node, ostream & os) const;

	private:
		CTNodePtr root_;
	};

	void TestCartesianTree();
}

#endif // !CARTESIAN_TREE_H
