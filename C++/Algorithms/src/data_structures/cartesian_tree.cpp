#include"cartesian_tree.h"
#include"../unit_test/unit_test.h"

#include<algorithm>

namespace tree
{
	ostream & operator<<(ostream & os, const CTNode & node)
	{
		os << node.value;
		return os;
	}

	ostream & operator<<(ostream & os, const KPPair& p)
	{
		os << p.key << "(" << p.priority << ") ";
		return os;
	}

	void CarTree::BuildTree(vector<KPPair> data)
	{
		// Sort <key, priority> values in increasing order
		sort(begin(data), end(data), [](const KPPair & left, const KPPair & right)
		{
			return left.key < right.key;
		});

		root_ = make_shared<CTNode>(*begin(data));

		CTNodePtr new_left_son = nullptr;
		bool is_root = false;

		// So the keys are in increasinr order now and initialy
		// we try to insert them as right sons of previous node
		for (auto cur_pair = next(begin(data)); cur_pair != end(data); ++cur_pair)
		{
			CTNodePtr cur_node = root_, par_node = root_;
			// Loop by the right path (because keys are in increasing order) until we
			// reach mismatch in priority condition
			while (cur_node && cur_node->value.priority > cur_pair->priority)
			{
				par_node = cur_node;
				cur_node = cur_node->right;
			}

			// Insert to the right tree path because all conditions are good
			if (par_node->value.priority > cur_pair->priority)
			{
				// Save right child of parent node, because it has lower priority
				// than inserted <key, priopity> pair
				CTNodePtr tmp = par_node->right;

				par_node->right = make_shared<CTNode>(*cur_pair);
				par_node->right->left = tmp;
			}
			else
			{
				// Inplace <key, priority> on par_node place, with copy of 
				// par_node node and make it left son of previously inserted 
				// node <key, priority>
				is_root = (par_node == root_) ? true : false;

				new_left_son = par_node;
				par_node = make_shared<CTNode>(*cur_pair);
				par_node->left = new_left_son;

				if (is_root)
				{
					root_ = par_node;
					is_root = false;
				}
			}
		}
	}

	void CarTree::Insert(const KPPair value)
	{
		// Split Treap by inserting value
		tuple<CTNodePtr, CTNodePtr> sub_trees = Split(root_, value.key);
		CTNodePtr ins_node = make_shared<CTNode>(value);

		if (!get<0>(sub_trees) && !get<1>(sub_trees))
			return;

		// If inserting value is lower than any of keys in Treap
		if (!get<0>(sub_trees))
			root_ = Merge(ins_node, root_);
		// If inserting value is higher than any of keys in Treap
		else if (!get<1>(sub_trees))
			root_ = Merge(root_, ins_node);
		// If inserting value in the middle of keys in Treap
		else
		{
			root_ = Merge(get<0>(sub_trees), ins_node);
			root_ = Merge(root_, get<1>(sub_trees));
		}
	}

	void CarTree::Remove(const KPPair value)
	{
		tuple<CTNodePtr, CTNodePtr> sub_trees = Split(root_, value.key);
		root_ = Merge(get<0>(sub_trees), get<1>(sub_trees));
	}

	bool CarTree::Search(const KPPair value)
	{
		return (SearchHelper(root_, value) != nullptr);
	}

	tuple<CTNodePtr, CTNodePtr> CarTree::Split(CTNodePtr & cur_node, const int & key)
	{
		if (!cur_node)
			return make_tuple(nullptr, nullptr);

		if (cur_node->value.key == key)
			return make_tuple(cur_node->left, cur_node->right);
		else if (key > cur_node->value.key)
		{
			tuple<CTNodePtr, CTNodePtr> tuple;
			if (cur_node->right)
				tuple = Split(cur_node->right, key);
			cur_node->right = get<0>(tuple);

			return make_tuple(cur_node, get<1>(tuple));
		}
		else
		{
			tuple<CTNodePtr, CTNodePtr> tuple;
			if (cur_node->left)
				tuple = Split(cur_node->left, key);
			cur_node->left = get<1>(tuple);

			return make_tuple(get<0>(tuple), cur_node);
		}
	}

	CTNodePtr CarTree::Merge(CTNodePtr first, CTNodePtr second)
	{
		if (!first)
			return second;
		if (!second)
			return first;

		if (first->value.priority > second->value.priority)
		{
			first->right = Merge(first->right, second);
			return first;
		}
		else
		{
			second->left = Merge(first, second->left);
			return second;
		}
	}

	CTNodePtr CarTree::SearchHelper(CTNodePtr cur_node, const KPPair value)
	{
		if (!cur_node)
			return nullptr;

		if (cur_node->value.key == value.key)
			return cur_node;
		else if (cur_node->value.key < value.key)
			return SearchHelper(cur_node->right, value);
		else
			return SearchHelper(cur_node->left, value);
	}

	void CarTree::InorderTraverse(CTNodePtr cur_node, ostream & os) const
	{
		if (cur_node->left)
			InorderTraverse(cur_node->left, os);

		if (cur_node == root_)
			os << " [ " << cur_node->value << "] ";
		else
			os << cur_node->value;

		if (cur_node->right)
			InorderTraverse(cur_node->right, os);
	}

	ostream & operator<<(ostream & os, const CarTree & ct)
	{
		ct.InorderTraverse(ct.root_, os);
		return os;
	}
	
	
	void TestCartesianTree()
	{
		CarTree ct;
		vector<KPPair> values = { { 20, 2 },{ 30, 3 },{ 40, 1 },{ 50, 5 },{ 60, 3 },{ 70, 1 },{ 80, 4 } };
		ct.BuildTree(values);
		for (const auto & el : values)
			unit_test::AssertEqual(true, ct.Search(el), "Cartesian Tree: After build tree search " + to_string(el.key));

		ct.Insert({ 90, 0 });
		unit_test::AssertEqual(true, ct.Search({ 90, 0 }), "Cartesian Tree: After Insert tree search highest element 90");
		ct.Insert({ 10, 0 });
		unit_test::AssertEqual(true, ct.Search({ 10, 0 }), "Cartesian Tree: After Insert tree search lowest element 10");
		ct.Insert({ 25, 2 });
		unit_test::AssertEqual(true, ct.Search({ 25, 0 }), "Cartesian Tree: After Insert tree search middle elements 25");

		ct.Remove({ 30, 3 });
		unit_test::AssertEqual(false, ct.Search({ 30, 3 }), "Cartesian Tree: After Remove tree search for 30");
	}
}