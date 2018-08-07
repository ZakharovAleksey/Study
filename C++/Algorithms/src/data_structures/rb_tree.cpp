#include"rb_tree.h"
#include"../unit_test/unit_test.h"

#include<algorithm>
#include<vector>

namespace tree
{
	ostream & operator<<(ostream & os, const RBNode & n)
	{
		char color = (n.c == Color::RED) ? 'R' : 'B';
		os << n.value << " (" << color  << ") ";
		return os;
	}

	ostream & operator<<(ostream & os, const RBTree & rbt)
	{
		rbt.InorderTraverse(rbt.root_, os);
		os << endl;
		return os;
	}

	void RBTreeTest()
	{
		RBTree rbt;
		vector<int> values = { 10, 5, 15, 7, 3, 2, 17, 16, 18, 19};
		vector<char> colors = { 'B', 'R', 'B', 'B', 'B', 'R', 'R', 'R', 'B', 'R' };

		for (const auto & el : values)
			rbt.Insert(el);

		for (size_t i = 0; i < values.size(); ++i)
		{
			auto exp_color = (rbt.Search(values[i])->c == Color::RED) ? 'R' : 'B';
			unit_test::AssertEqual(colors[i], exp_color, "RB-Tree: Check color from node " + to_string(values[i]) + ".");
		}
		
		unit_test::AssertEqual(true, rbt.Find(5), "RB-Tree: Find value which is in tree");
		unit_test::AssertEqual(false, rbt.Find(-5), "RB-Tree: Find value which is NOT in tree: little");
		unit_test::AssertEqual(false, rbt.Find(8), "RB-Tree: Find value which is NOT in tree: medium");
		unit_test::AssertEqual(false, rbt.Find(20), "RB-Tree: Find value which is NOT in tree: high");

		unit_test::AssertEqual(4, rbt.MaxDepth(), "RB-Tree: Max depth before Insert(20)");
		rbt.Insert(20);
		unit_test::AssertEqual(5, rbt.MaxDepth(), "RB-Tree: Max depth after Insert(20)");
	}

	RBTree::RBTree() : root_(nullptr) { }

	void RBTree::Insert(const int & value)
	{
		RBNodePtr cur_node = InsertBST(value);

		if (cur_node == root_)
			root_->c = Color::BLACK;
		else if (cur_node->parent->parent)
			FixBalance(cur_node);
	}

	bool RBTree::Find(const int & value)
	{
		return (SearchHelper(root_, value) != nullptr);
	}

	RBNodePtr RBTree::Search(const int & value)
	{
		return SearchHelper(root_, value);
	}

	RBNodePtr RBTree::SearchHelper(RBNodePtr cur_node, const int & value)
	{
		if (!cur_node)
			return nullptr;

		if (value == cur_node->value)
			return cur_node;
		else if (value < cur_node->value)
			return SearchHelper(cur_node->left, value);
		else if (value > cur_node->value)
			return SearchHelper(cur_node->right, value);
	}

	size_t RBTree::MaxDepth() const
	{
		return MaxDepthHelper(root_);
	}

	void RBTree::FixBalance(RBNodePtr cur_node)
	{
		while (cur_node != root_ && cur_node->c != Color::BLACK && cur_node->parent->c == Color::RED)
		{
			RBNodePtr par_node = cur_node->parent;
			RBNodePtr gr_node = cur_node->parent->parent;

			if (gr_node)
			{
				if (par_node == gr_node->left)
				{
					RBNodePtr un_node = gr_node->right;

					if (un_node && un_node->c == Color::RED)
						cur_node = LeftRedUncle(cur_node, par_node, gr_node, un_node);
					else // Here we do not need if because if un_node is nullptr -> it is BLACK
					{
						if (cur_node == par_node->left)
							cur_node = LeftLeftCase(cur_node, par_node, gr_node, un_node);
						else if (cur_node == par_node->right)
							cur_node = LeftRightCase(cur_node, par_node, gr_node, un_node);
					}
				}
				else
				{
					RBNodePtr un_node = gr_node->left;

					if (un_node && un_node->c == Color::RED)
						cur_node = RightRedUncle(cur_node, par_node, gr_node, un_node);
					else // Here we do not need if because if un_node is nullptr -> it is BLACK
					{
						if (cur_node == par_node->right)
							cur_node = RightRightCase(cur_node, par_node, gr_node, un_node);
						else if (cur_node == par_node->left)
							cur_node = RightLeftCase(cur_node, par_node, gr_node, un_node);
					}
				}
			}
		}

		root_->c = Color::BLACK;
	}

	RBNodePtr RBTree::LeftRedUncle(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node)
	{
		gr_node->c = Color::RED;
		par_node->c = Color::BLACK;
		un_node->c = Color::BLACK;

		return gr_node;
	}

	RBNodePtr RBTree::RightRedUncle(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node)
	{
		gr_node->c = Color::RED;
		par_node->c = Color::BLACK;
		un_node->c = Color::BLACK;
		
		return gr_node;
	}

	RBNodePtr RBTree::LeftLeftCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node)
	{
		RotateRight(gr_node);
		swap(gr_node->c, par_node->c);

		return par_node;
	}

	RBNodePtr RBTree::RightRightCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node)
	{
		RotateLeft(gr_node);
		swap(gr_node->c, par_node->c);
		
		return par_node;
	}

	RBNodePtr RBTree::LeftRightCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node)
	{
		RotateLeft(par_node);
		return LeftLeftCase(cur_node, par_node, gr_node, un_node);
	}

	RBNodePtr RBTree::RightLeftCase(RBNodePtr cur_node, RBNodePtr par_node, RBNodePtr gr_node, RBNodePtr un_node)
	{
		RotateRight(par_node);
		return RightRightCase(cur_node, par_node, gr_node, un_node);
	}

	void RBTree::InorderTraverse(RBNodePtr cur_node, ostream & os) const
	{
		if (cur_node->left)
			InorderTraverse(cur_node->left, os);

		if (cur_node != root_)
			os << *cur_node;
		else
			os << "[ " << *cur_node << " ] ";

		if (cur_node->right)
			InorderTraverse(cur_node->right, os);
	}

	RBNodePtr RBTree::InsertBST(const int & value)
	{
		if (!root_)
		{
			root_ = make_shared<RBNode>(value);
			return root_;
		}
		
		RBNodePtr cur_node = root_, par_node = nullptr;
		while (cur_node)
		{
			par_node = cur_node;
			if (value < cur_node->value)
				cur_node = cur_node->left;
			else if (value > cur_node->value)
				cur_node = cur_node->right;
			else
				return cur_node;
		}
		
		if (value < par_node->value)
		{
			par_node->left = make_shared<RBNode>(value);
			par_node->left->parent = par_node;
			return par_node->left;
		}
		else
		{
			par_node->right = make_shared<RBNode>(value);
			par_node->right->parent = par_node;
			return par_node->right;
		}
	}

	size_t RBTree::MaxDepthHelper(RBNodePtr cur_node) const
	{
		size_t left = 0, right = 0;
		if (!cur_node)
			return 0;

		if (cur_node->left)
			left = MaxDepthHelper(cur_node->left);
		if (cur_node->right)
			right = MaxDepthHelper(cur_node->right);

		return max(left, right) + 1;
	}

	void RBTree::RotateRight(RBNodePtr y)
	{
		RBNodePtr x = y->left, alpha = x->left, betta = x->right, jamma = y->right;
		swap(x->value, y->value);
		swap(x->c, y->c);

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
	}

	void RBTree::RotateLeft(RBNodePtr y)
	{
		RBNodePtr x = y->right, alpha = y->left, betta = x->left, jamma = x->right;
		swap(x->value, y->value);
		swap(x->c, y->c);

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
	}
}