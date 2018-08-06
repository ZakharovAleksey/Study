#include"b_tree.h"
#include"../unit_test/unit_test.h"

#include<algorithm>
#include<iterator>

#include<sstream>

namespace tree
{
	BTreeNode::BTreeNode(size_t _min_degree, bool _is_leaf) : 
		el_numb(0), min_degree(_min_degree), is_leaf(_is_leaf) {}

	BTreeNodePtr BTreeNode::Search(const int & value)
	{
		auto it = lower_bound(begin(keys), end(keys), value);

		if (it != end(keys) && *it == value)
			return make_shared<BTreeNode>(*this);

		if (is_leaf)
			return nullptr;

		return body[distance(begin(keys), it)]->Search(value);
	}

	void BTreeNode::InsertToNonFullNode(const int & value)
	{
		if (is_leaf)
		{
			auto pos = lower_bound(begin(keys), end(keys), value);
			keys.insert(pos, value);
			++el_numb;
		}
		else
		{
			auto next_node = lower_bound(begin(keys), end(keys), value);
			// Two cases:
			// - If we insert lowest element : go to body[0] (0 id is equal to begin(keys))
			// - If we insert highest element : go to body[keys_numb] (keys_numb id is equal to end(keys))
			// - In other cases insert to position PREVIOUS to lower_bound()
			next_node = (next_node != begin(keys) && next_node != end(keys)) ? prev(next_node) : next_node;
			size_t id_ins = distance(begin(keys), next_node);

			// Check if node for insertion is full with values
			if (body[id_ins]->el_numb == 2 * min_degree - 1)
			{
				SplitNode(id_ins, body[id_ins]);
				// Here we split our node on two: left and right
				// So, here we check in which node we will make insertion : 
				// left (id = id_ins) or right (id = id_ins + 1)
				if (value > keys[id_ins])
					++id_ins;
			}
			body[id_ins]->InsertToNonFullNode(value);
		}
	}

	void BTreeNode::SplitNode(int cur_node_id, BTreeNodePtr cur_node)
	{
		// Create new node which stores highest values
		BTreeNodePtr new_node = make_shared<BTreeNode>(cur_node->min_degree, cur_node->is_leaf);
		new_node->el_numb = min_degree - 1;

		// Copy highest (min_degree + 1) keys to new_node
		copy(end(cur_node->keys) - min_degree + 1, end(cur_node->keys), back_inserter(new_node->keys));
		cur_node->keys.erase(end(cur_node->keys) - min_degree + 1, end(cur_node->keys));

		// Copy highest (min_degree) nodes which corresponds
		// to already copied (min degree + 1) keys to new_node
		if (!cur_node->is_leaf)
		{
			copy(end(cur_node->body) - min_degree, end(cur_node->body), back_inserter(new_node->body));
			cur_node->body.erase(end(cur_node->body) - min_degree, end(cur_node->body));
		}

		cur_node->el_numb = min_degree - 1;

		// Insert previosly created new_node right on position after current node
		body.insert(begin(body) + cur_node_id + 1, new_node);
		// Insert new separate key for pair of already created lists
		// This must be the highest value from cur_node (by the way last element), 
		// because the result should be:
		//		  separate key			[To level_UP node we add new separate key]
		//		/				\
		//	[cur_node]		[new_node]
		keys.insert(begin(keys) + cur_node_id, *rbegin(cur_node->keys));
		cur_node->keys.pop_back();
		++el_numb;

		//assert(cur_node->keys.size() == cur_node->el_numb);
		//assert(new_node->keys.size() == new_node->el_numb);
	}

	ostream & operator<<(ostream & os, const BTreeNode & node)
	{
		os << "[ ";
		for (auto it = begin(node.keys); it != end(node.keys); ++it)
		{
			// Previouslly loop over the childs
			if (!node.is_leaf)
				os << *node.body[distance(begin(node.keys), it)] << " ";
			// Than cout current key
			os << *it << " ";
		}

		// Loop over the last key
		if (node.body.size() > 0 && *rbegin(node.body))
			os << **rbegin(node.body) << " ";
		os << " ]";
		return os;
	}

	ostream & operator<<(ostream & os, const BTree & bt)
	{
		if (bt.root_)
			os << *bt.root_ << endl;
		return os;
	}

	BTree::BTree(size_t min_degree) : root_(nullptr), min_degree_(min_degree) {}

	BTreeNodePtr BTree::Search(const int & value)
	{
		return (root_ == nullptr) ? nullptr : root_->Search(value);
	}

	bool BTree::Find(const int & value)
	{
		return (Search(value) != nullptr);
	}

	void BTree::Insert(const int & value)
	{
		if (!root_)
		{
			root_ = make_shared<BTreeNode>(min_degree_, true);
			root_->keys.push_back(value);
			root_->el_numb = 1;
		}
		else
		{
			if (root_->el_numb == 2 * min_degree_ - 1)
			{
				BTreeNodePtr new_root = make_shared<BTreeNode>(min_degree_, false);

				// Add new empty node to end of root and Split this last node()
				// Here picture:
				//		[new_root] -> empty	
				//			|
				//		 [root_]
				new_root->body.insert(begin(new_root->body), root_);
				//size_t last_body_id = new_root->body.size() - 1;
				new_root->SplitNode(0, root_);
				// As a result of split we obtain:
				//		[new_root] -> key number increase by 1
				//		 /		\
				//	[root_]	   [new_node]

				// Here we split our node on two: left and right
				// So, here we check in which node we will make insertion : 
				// left (id = id_ins) or right (id = id_ins + 1)
				size_t id_ins = (new_root->keys[0] < value) ? 1 : 0;
				new_root->body[id_ins]->InsertToNonFullNode(value);

				root_ = new_root;
			}
			else
				root_->InsertToNonFullNode(value);
		}
	}

	void BTreeTest()
	{
		BTree bt(3);
		vector<int> v = { 3, 10, 20, 5, 6, 12, 30, 7, 17 };
		for (const auto & el : v)
			bt.Insert(el);

		ostringstream oss;
		for (const auto & el : v)
		{
			bool a = bt.Find(el);
			unit_test::AssertEqual(bt.Find(el), true, "BTree: Find element, existing in tree (" + to_string(el) + ")");
			oss << el << ", ";
		}

		string exp_output = "3, 10, 20, 5, 6, 12, 30, 7, 17, ";
		unit_test::AssertEqual(exp_output, oss.str(), "BTree: output in increasing order");

		unit_test::AssertEqual(false, bt.Find(-1), "BTree: Find element, NOT existing in tree low");
		unit_test::AssertEqual(false, bt.Find(9), "BTree: Find element, NOT existing in tree middle");
		unit_test::AssertEqual(false, bt.Find(100), "BTree: Find element, NOT existing in tree hight");
	}
}