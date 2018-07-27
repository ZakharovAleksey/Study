#include"splay_tree.h"

#include"../unit_test/unit_test.h"
#include<vector>

namespace tree
{
	ostream & operator<<(ostream & os, const NodeST & node)
	{
		os << node.value << " ";
		return os;
	}

	ostream & operator<<(ostream & os, const SplayTree & st)
	{
		if (st.IsEmpty())
			os << "SplayTree is empty" << endl;
		else
		{
			st.InorderTraversal(st.root_, os);
			os << endl;
		}
		return os;
	}

	SplayTree::SplayTree() : root_(nullptr) {}

	void SplayTree::Insert(const int & value)
	{
		if (!root_)
		{
			root_ = make_shared<NodeST>(value);
			return;
		}

		NodeStPtr cur_node = root_, par_node = nullptr;
		while (cur_node)
		{
			par_node = cur_node;
			if (value < cur_node->value)
				cur_node = cur_node->left;
			else if (value > cur_node->value)
				cur_node = cur_node->right;
			else
				return;
		}

		if (value < par_node->value)
		{
			par_node->left = make_shared<NodeST>(value);
			par_node->left->parent = par_node;
			Splay(par_node->left);
		}
		else
		{
			par_node->right = make_shared<NodeST>(value);
			par_node->right->parent = par_node;
			Splay(par_node->right);
		}
	}

	bool SplayTree::Find(const int & value)
	{
		NodeStPtr cur_node = Find(root_, value);
		if (cur_node)
			Splay(cur_node);

		return (cur_node != nullptr);
	}

	void SplayTree::Remove(const int & value)
	{
		NodeStPtr cur_node = Find(root_, value);
		
		if (!cur_node)
			return;
		Splay(cur_node);
		// Case 1. Root has no sons (left AND right)
		if (!root_->left && !root_->right)
			root_.reset();

		// Case 2. Root has ONLY one son (left or right)
		else if (!root_->left && root_->right)
		{
			NodeStPtr right_son = root_->right;
			root_.reset();
			root_ = right_son;
			root_->parent = nullptr;
		}
		else if (!root_->right && root_->left)
		{
			NodeStPtr left_son = root_->left;
			root_.reset();
			root_ = left_son;
			root_->parent = nullptr;
		}
		else
			SplitAndMerge();
	}

	bool SplayTree::IsEmpty() const
	{
		return (!root_);
	}

	int SplayTree::GetRootValue() const
	{
		return root_->value;
	}

	size_t SplayTree::GetDepth() const
	{
		return GetDepth(root_);
	}

	void SplayTree::InorderTraversal(NodeStPtr cur_node, ostream & os) const
	{
		if (cur_node->left)
			InorderTraversal(cur_node->left);

		os << *cur_node;
		if (cur_node == root_)
			os << " (root) ";

		if (cur_node->right)
			InorderTraversal(cur_node->right);
	}

	size_t SplayTree::GetDepth(NodeStPtr cur_node) const
	{
		if (!cur_node)
			return 0;
		size_t left_depth = GetDepth(cur_node->left);
		size_t right_depth = GetDepth(cur_node->right);

		return (left_depth < right_depth) ? right_depth + 1 : left_depth + 1;
	}

	NodeStPtr SplayTree::Find(NodeStPtr cur_node, const int & value) const
	{
		if (!cur_node || cur_node->value == value)
			return cur_node;
		else if (value < cur_node->value)
			return Find(cur_node->left, value);

		return Find(cur_node->right, value);
	}

	void SplayTree::SplitAndMerge()
	{
		/*
			Remove root with two childs explanation:
			1. Create two sub-trees: left and right (root is out of game - we remove it)
			2. Find maximum value in left sub tree and Splay() it in LEFT sub-tree
			As a result in the root of LEFT sub tree will be maximal element:
				- It has no right son (because this is max value)
			All values in LEFT sub-tree less than all elements in RIGHT sub-tree
			3. Add right sub-tree as right son of rool for LEFT sub-tree
		*/

		SplayTree l_sub_tree(root_->left);
		SplayTree r_sub_tree(root_->right);
		l_sub_tree.root_->parent = r_sub_tree.root_->parent = nullptr;

		NodeStPtr l_max_el = l_sub_tree.root_;
		while (l_max_el->right)
			l_max_el = l_max_el->right;
		l_sub_tree.Splay(l_max_el);
		l_sub_tree.root_->right = r_sub_tree.root_;

		root_ = l_sub_tree.root_;
	}

	void SplayTree::Splay(NodeStPtr cur_node)
	{
		while (true)
		{
			NodeStPtr parent = cur_node->parent;

			// If we already in the root
			if (!parent)
				break;

			// If parent of current node is root
			NodeStPtr gr_parent = parent->parent;
			if (!gr_parent)
			{
				if (cur_node == parent->left)
					cur_node = ZigRotation(parent);
				else
					cur_node = ZagRotation(parent);
			}

			if (gr_parent)
			{
				if (gr_parent->left && gr_parent->left == parent)
				{
					if (parent->left && parent->left == cur_node)
					{
						cur_node = ZigRotation(gr_parent);
						cur_node = ZigRotation(cur_node);
					}
					else if (parent->right && parent->right == cur_node)
					{
						cur_node = ZagRotation(parent);
						cur_node = ZigRotation(gr_parent);
					}
				}
				else
				{
					if (parent->left && parent->left == cur_node)
					{
						cur_node = ZigRotation(parent);
						cur_node = ZagRotation(gr_parent);
					}
					else if (parent->right && parent->right == cur_node)
					{
						cur_node = ZagRotation(gr_parent);
						cur_node = ZagRotation(cur_node);
					}
				}
			}
		}
	}

	NodeStPtr SplayTree::ZigRotation(NodeStPtr & y)
	{
		NodeStPtr x = y->left, alpha = x->left, betta = x->right, jamma = y->right;

		swap(x->value, y->value);
		y->left = alpha;

		if (alpha)
			alpha->parent = y;
		y->right = x;
		x->parent = y;

		x->left = betta;
		if (betta)
			betta->parent = x;

		x->right = jamma;
		if (jamma)
			jamma->parent = x;

		return y;
	}

	NodeStPtr SplayTree::ZagRotation(NodeStPtr & y)
	{
		NodeStPtr x = y->right, alpha = y->left, betta = x->left, jamma = x->right;
		swap(x->value, y->value);

		y->right = jamma;
		if (jamma)
			jamma->parent = y;

		y->left = x;
		x->parent = y;

		x->left = alpha;
		if (alpha)
			alpha->parent = x;

		x->right = betta;
		if (betta)
			betta->parent = x;

		return y;
	}

	
	void SplayTreeTest()
	{
		SplayTree st;
		vector<int> elements =  { 0, 2, 1, 6, 4, 5 };
		vector<size_t> depths = { 1, 2, 2, 4, 4, 5 };
		
		for(auto it = begin(elements); it != end(elements); ++it)
		{
			st.Insert(*it);
			unit_test::AssertEqual(st.GetDepth(), *(begin(depths) + distance(begin(elements), it)), " SplayTree: tree depth test");
			unit_test::AssertEqual(st.GetRootValue(), *it, " SplayTree: root value test");
		}

		unit_test::AssertEqual(st.Find(-1), false, "Splay Tree: not find value test");
		unit_test::AssertEqual(st.Find(4), true, "Splay Tree: find value test");

		for (const auto & el : elements)
			st.Remove(el);

		unit_test::AssertEqual(st.IsEmpty(), true, " SplayTree: remove all elements");
	}

}