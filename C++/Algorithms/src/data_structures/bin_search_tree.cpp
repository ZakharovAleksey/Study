#include"bin_search_tree.h"

#include"../unit_test/unit_test.h"
#include<vector>

namespace tree
{
	ostream & operator<<(ostream & os, const NodeBST & node)
	{
		os << node.value << " ";
		return os;
	}

	ostream & operator<<(ostream & os, const BSTree & st)
	{
		if (st.IsEmpty())
			os << "BSTree is empty" << endl;
		else
		{
			st.InorderTraversal(st.root_, os);
			os << endl;
		}
		return os;
	}

	BSTree::BSTree() : root_(nullptr) {}

	void BSTree::Insert(const int & value)
	{
		if (!root_)
		{
			root_ = make_shared<NodeBST>(value);
			return;
		}

		NodeBSTPtr cur_node = root_, par_node = nullptr;
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
			par_node->left = make_shared<NodeBST>(value);
			par_node->left->parent = par_node;
		}
		else
		{
			par_node->right = make_shared<NodeBST>(value);
			par_node->right->parent = par_node;
		}
	}

	bool BSTree::Find(const int & value)
	{
		return (Find(root_, value) != nullptr);
	}

	void BSTree::Remove(const int & value)
	{
		NodeBSTPtr cur_node = Find(root_, value);
		if (cur_node)
			Remove(cur_node);
	}

	bool BSTree::IsEmpty() const
	{
		return (!root_);
	}

	size_t BSTree::GetDepth() const
	{
		return GetDepth(root_);
	}

	void BSTree::InorderTraversal(NodeBSTPtr cur_node, ostream & os) const
	{
		if (cur_node->left)
			InorderTraversal(cur_node->left);

		os << *cur_node;
		if (cur_node == root_)
			os << " (root) ";

		if (cur_node->right)
			InorderTraversal(cur_node->right);
	}

	size_t BSTree::GetDepth(NodeBSTPtr cur_node) const
	{
		if (!cur_node)
			return 0;
		size_t left_depth = GetDepth(cur_node->left);
		size_t right_depth = GetDepth(cur_node->right);

		return (left_depth < right_depth) ? right_depth + 1 : left_depth + 1;
	}

	NodeBSTPtr BSTree::Find(NodeBSTPtr cur_node, const int & value) const
	{
		if (!cur_node || cur_node->value == value)
			return cur_node;
		else if (value < cur_node->value)
			return Find(cur_node->left, value);

		return Find(cur_node->right, value);
	}

	void BSTree::Remove(NodeBSTPtr cur_node)
	{
		// Additional check on root removement
		if (cur_node == root_)
		{
			root_.reset();
			return;
		}

		if (!cur_node)
			return;

		// If current node is a leaf
		if (!cur_node->left && !cur_node->right)
			RemoveLeaf(cur_node);

		// If current node has ONLY one child: left or right
		else if ((cur_node->left && !cur_node->right) || (!cur_node->left && cur_node->right))
			RemoveOneChild(cur_node);

		// If current node has both childs
		else
		{
			NodeBSTPtr predessor = Predecessor(cur_node);
			swap(cur_node->value, predessor->value);
			Remove(predessor);
		}
	}

	void BSTree::RemoveLeaf(NodeBSTPtr cur_node)
	{
		NodeBSTPtr parent = cur_node->parent;

		if (cur_node == parent->left)
		{
			cur_node.reset();
			parent->left = nullptr;
		}
		else
		{
			cur_node.reset();
			parent->right = nullptr;
		}
	}

	void BSTree::RemoveOneChild(NodeBSTPtr cur_node)
	{
		NodeBSTPtr parent = cur_node->parent;
		NodeBSTPtr existed_son = (cur_node->left) ? cur_node->left : cur_node->right;

		if (cur_node == root_)
		{
			if (!root_->left && root_->right)
			{
				NodeBSTPtr right_son = root_->right;
				root_.reset();
				root_ = right_son;
				root_->parent = nullptr;
			}
			else if (!root_->right && root_->left)
			{
				NodeBSTPtr left_son = root_->left;
				root_.reset();
				root_ = left_son;
				root_->parent = nullptr;
			}
		}
		else
		{
			if (cur_node == parent->left)
			{
				cur_node.reset();
				parent->left = existed_son;
				existed_son->parent = parent;
			}
			else
			{
				cur_node.reset();
				parent->right = existed_son;
				existed_son->parent = parent;
			}
		}
	}

	NodeBSTPtr BSTree::Predecessor(NodeBSTPtr cur_node)
	{
		NodeBSTPtr predecessor = cur_node->left;
		while (predecessor->right)
			predecessor = predecessor->right;

		return predecessor;
	}

	void BSTreeTest()
	{
		BSTree bst;
		vector<int> elements =  { 0, 2, 1, 6, 4, 5 };
		vector<size_t> depths = { 1, 2, 3, 3, 4, 5 };

		for (auto it = begin(elements); it != end(elements); ++it)
		{
			bst.Insert(*it);
			unit_test::AssertEqual(bst.GetDepth(), *(begin(depths) + distance(begin(elements), it)), " BSTree: tree depth test");
		}

		unit_test::AssertEqual(bst.Find(-1), false, "BSTree: not find value test");
		unit_test::AssertEqual(bst.Find(4), true, "BSTree: find value test");

		for (const auto & el : elements)
			bst.Remove(el);

		unit_test::AssertEqual(bst.IsEmpty(), true, " BSTree: remove all elements");
	}

}