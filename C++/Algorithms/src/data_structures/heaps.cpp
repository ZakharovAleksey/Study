#include"heaps.h"
#include"../unit_test/unit_test.h"


#include<algorithm>
#include<iterator>
#include<climits>

namespace heap
{
	BinHeap::BinHeap() {}

	VecIt BinHeap::Insert(const int & value)
	{
		if (body_.empty())
		{
			body_.push_back(value);
			return begin(body_);
		}
		body_.push_back(value);
		return SiftUp(prev(end(body_)));
	}

	int BinHeap::ExtractMin()
	{
		if (body_.empty())
			return -1;

		int min = *begin(body_);

		if (body_.size() == 1)
			body_.pop_back();
		else
		{
			swap(*begin(body_), *prev(end(body_)));
			body_.pop_back();
			SiftDown();
		}

		return min;
	}

	VecIt BinHeap::DecreaseKey(VecIt key_pos, const int & new_value)
	{
		if (!body_.empty() && key_pos < end(body_))
		{
			*key_pos = new_value;
			return SiftUp(key_pos);
		}

		return end(body_);
	}

	VecIt BinHeap::Begin()
	{
		return begin(body_);
	}

	VecIt BinHeap::SiftUp(VecIt cur_it)
	{
		auto par_it = begin(body_) + static_cast<size_t>(floorf(distance(begin(body_), cur_it) - 1.f) / 2.f);
		while (cur_it != begin(body_) && *par_it > *cur_it)
		{
			swap(*cur_it, *par_it);
			cur_it = par_it;
			par_it = begin(body_) + static_cast<size_t>(floorf(distance(begin(body_), cur_it) - 1.f) / 2.f);
		}
		return cur_it;
	}

	VecIt BinHeap::SiftDown()
	{
		VecIt cur_it = begin(body_), left_it = GetLeftChild(cur_it), right_it = GetRightChild(cur_it);

		VecIt min_it = GetMin(left_it, right_it);

		while (min_it != end(body_) && cur_it != end(body_) && *cur_it > *min_it)
		{
			swap(*cur_it, *min_it);

			cur_it = min_it;
			left_it = GetLeftChild(cur_it); right_it = GetRightChild(cur_it);
			
			min_it = GetMin(left_it, right_it);
		}
		return cur_it;
	}

	VecIt BinHeap::GetLeftChild(VecIt cur_it)
	{
		size_t offset = 2 * distance(begin(body_), cur_it) + 1;
		return  (offset < body_.size()) ? begin(body_) + offset : end(body_);
	}

	VecIt BinHeap::GetRightChild(VecIt cur_it)
	{
		size_t offset = 2 * distance(begin(body_), cur_it) + 2;
		return  (offset < body_.size()) ? begin(body_) + offset : end(body_);
	}

	VecIt BinHeap::GetMin(VecIt left_it, VecIt right_it)
	{
		if (left_it != end(body_) && right_it != end(body_))
			return (*left_it < *right_it) ? left_it : right_it;
		else if (left_it == end(body_) && right_it != end(body_))
			return right_it;
		else if (left_it != end(body_) && right_it == end(body_))
			return left_it;
		else
			return end(body_);
	}

	ostream & operator<<(ostream & os, const BinHeap & bh)
	{
		copy(begin(bh.body_), end(bh.body_), ostream_iterator<int>(os, " "));
		os << endl;
		return os;
	}

	void BinHeapTest()
	{
		heap::BinHeap bh;
		vector<int> values = { 10, 7 , 15, 12, 17 };
		vector<int> tmp(values);
		for (const auto & el : values)
			bh.Insert(el);

		make_heap(begin(tmp), end(tmp));
		
		for (size_t i = 0; i < tmp.size(); ++i)
			unit_test::AssertEqual(tmp[tmp.size() - 1 - i], *(bh.Begin() + i), "Binary Heap: Bin Heap is equal to make_heap()");

		sort(begin(tmp), end(tmp));
		for (const auto & cur_min : tmp)
			unit_test::AssertEqual(cur_min, bh.ExtractMin(), "Binary Heap: Extract Minimum value " + to_string(cur_min));
	}

	ostream & operator<<(ostream & os, const BinomTreeNode & btn)
	{
		os << btn.value << " ";
		for (const auto & node : btn.childs)
			os << *node;
		return os;
	}

	ostream & operator<<(ostream & os, const BinomTree & bt)
	{
		os << "degree = " << bt.degree_ << endl << *bt.root_ << endl;
		return os;
	}

	ostream & operator<<(ostream & os, const BinomHeap & bh)
	{
		copy(begin(bh.bin_trees_), end(bh.bin_trees_), ostream_iterator<BinomTree>(os));
		os << endl;
		return os;
	}

	void BinomHeapTest()
	{
		heap::BinomHeap bh;
		vector<int> values = { -1, 2 , 5, 4, 3, 10, 1 };
		vector<int> tmp(values);
		for (const auto & el : values)
			bh.Insert(el);

		sort(begin(tmp), end(tmp));
		for (const auto & cur_min : tmp)
		{
			unit_test::AssertEqual(cur_min, bh.GetMin(), "Binominal Heap: Extract Minimum value " + to_string(cur_min));
			bh.ExtractMin();
		}
	}

	BinomTreeNode::BinomTreeNode(int _value) : value(_value) {}

	BinomTree::BinomTree(int value) : root_(make_shared<BinomTreeNode>(value)), degree_(0) {}

	BinomTree::BinomTree(BinomTreeNodePtr root, size_t degree) : root_(root), degree_(degree) {}

	void BinomTree::MeltSimilarDegree(const BinomTree & other)
	{
		// Choose minimum from two roots and attach BinomTree with max
		// root value as a child of BinomTree with minimal root value
		if (root_->value < other.root_->value)
			root_->childs.push_back(other.root_); // move
		else
		{
			other.root_->childs.push_back(root_); // move
			root_ = other.root_;
		}
		++degree_;
	}
	

	BinomHeap::BinomHeap() : min_(INT_MAX) {}

	void BinomHeap::Insert(const int & value)
	{
		// Update minimum element
		min_ = (value < min_) ? value : min_;

		// Create Binomial Tree of size 1 and insert it to the begining of 
		// list of Binomial Trees
		BinomTree ins_tree = BinomTree(value);
		bin_trees_.push_front(ins_tree);

		auto cur_tree_it = begin(bin_trees_), next_tree_it = next(cur_tree_it);

		// While dergees of two neighbouring nodes are equal -> Melt(cur, next).
		// Explain:
		// We Insert BinomTree size 1 to the begining of BinomTree list:
		// size 1 + size 1 -> size 2 : Now cur_it point to BinomTree size 2 
		//		- If we have another tree size 2 (It is next to this) -> Melt(cur, next)
		//		- Otherwise stop loop. Nothing ot Melt()
		while (next_tree_it != end(bin_trees_) && cur_tree_it->degree_ == next_tree_it->degree_)
		{
			cur_tree_it->MeltSimilarDegree(*next_tree_it);
			bin_trees_.erase(next_tree_it);
			next_tree_it = next(cur_tree_it);
		}
	}

	void BinomHeap::ExtractMin()
	{
		// Find pointer to min element
		auto min_it = find_if(begin(bin_trees_), end(bin_trees_), [&](const BinomTree & bt)
		{
			return bt.root_->value == min_;
		});
		// Create BinomTrees from roots of min elements childs
		vector<BinomTree> min_el_childs;
		for_each(begin(min_it->root_->childs), end(min_it->root_->childs),
			[&min_el_childs](const BinomTreeNodePtr & p)
		{
			min_el_childs.push_back(BinomTree(p, p->childs.size())); // p->childs.size() - number of degrees !!!
		});

		std::sort(begin(min_el_childs), end(min_el_childs),
			[](const BinomTree & left, const BinomTree & right) -> bool
		{
			return left.degree_ < right.degree_;
		});

		// Remove BinomTree with minimal element
		min_it->root_->childs.clear();
		bin_trees_.erase(min_it);

		// Melt new BinomTrees with current BinHeap BinomTrees
		for (auto & ch : min_el_childs)
		{
			// Find position to inserte current child (based on degree of child BinomTree)
			auto ins_pos = find_if(begin(bin_trees_), end(bin_trees_), [&ch](const BinomTree & bt) -> bool
			{
				return bt.degree_ == ch.degree_;
			});
			ins_pos = (ins_pos == begin(bin_trees_)) ? begin(bin_trees_) : prev(ins_pos);

			// Indsert child BinomTree to appropriate position
			bin_trees_.insert(ins_pos, ch);
			ins_pos = prev(ins_pos);

			// Perform Melt is nesessary like in Insert() method
			if (ins_pos != end(bin_trees_))
			{
				auto next_pos = next(ins_pos);

				while (next_pos != end(bin_trees_) && ins_pos->degree_ == next_pos->degree_)
				{
					ins_pos->MeltSimilarDegree(*next_pos);
					bin_trees_.erase(next_pos);
					next_pos = next(ins_pos);
				}
			}
		}

		// Update minimal element
		min_ = INT_MAX;
		for_each(begin(bin_trees_), end(bin_trees_), [&](const BinomTree & bt)
		{
			min_ = min(bt.root_->value, min_);
		});
	}

	int BinomHeap::GetMin() const
	{
		return min_; 
	}

}