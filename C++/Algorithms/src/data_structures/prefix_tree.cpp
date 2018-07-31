#include"prefix_tree.h"

#include"../unit_test/unit_test.h"

#include<iostream>
#include<algorithm>

namespace tree
{
	TrieTree::TrieTree() : root_(new TrieNode()) {}

	void TrieTree::Insert(const string & word)
	{
		TrieNodePtr cur_node = root_;
		for (auto symb_it = begin(word); symb_it != end(word); ++symb_it)
		{
			cur_node->body.insert({ *symb_it, make_shared<TrieNode>() });
			cur_node = cur_node->body[*symb_it];
			//cur_node = (symb_it == prev(end(word))) ? cur_node : cur_node->body[*symb_it];
		}
		cur_node->is_complete_word = true;
	}

	bool TrieTree::Search(const string & word)
	{
		TrieNodePtr cur_node = root_;
		for (auto symb_it = begin(word); symb_it != end(word); ++symb_it)
		{
			auto next_pos = find_if(begin(cur_node->body), end(cur_node->body), [&symb_it](const pair<char, TrieNodePtr> & p)
			{
				return p.first == *symb_it;
			});

			if (next_pos != end(cur_node->body))
				cur_node = cur_node->body[*symb_it];
			//cur_node = (symb_it == prev(end(word))) ? cur_node : cur_node->body[*symb_it];
			else
				return false;
		}
		return cur_node->is_complete_word;
	}

	void TrieTree::Remove(const string & word)
	{
		DeleteHelper(root_, word, begin(word));
	}

	bool TrieTree::IsEmpty() const
	{
		return root_->body.size() == 0;
	}

	bool TrieTree::DeleteHelper(TrieNodePtr cur_node, const string & word, string::const_iterator cur_symb_it)
	{
		if (cur_node->body.empty() || word.empty() || cur_symb_it == end(word))
			return false;

		// Find if current word's symbol is in hash map of current layer
		auto cur_layer_it = find_if(begin(cur_node->body), end(cur_node->body), [&cur_symb_it](const pair<char, TrieNodePtr> & p)
		{
			return p.first == *cur_symb_it;
		});

		if (cur_layer_it != end(cur_node->body))
		{
			// If we get to the last symbol of the word
			if (cur_symb_it == prev(end(word)))
			{
				// If this is the ONLY symbol on this layer
				//cur_node->is_complete_word = false;
				cur_layer_it->second->is_complete_word = false;

				// Set node to remove if it has no child
				if (cur_layer_it->second->body.empty())
					return true;
				return false;
			}
			else
			{
				if (DeleteHelper(cur_layer_it->second, word, next(cur_symb_it)))
				{
					// Remove node from hash_map on current layer if it is unique
					cur_layer_it->second->body.erase(*next(cur_symb_it));

					return cur_node->body.empty() && !cur_node->is_complete_word;
				}
			}
		}

		return false;
	}

	void TrieTreeTest()
	{
		vector<string> words = { "the", "there", "their", "a", "answer", "any", "by", "bye", "lol", "loh" };

		tree::TrieTree t;
		for (const auto & w : words)
			t.Insert(w);

		for (const auto & w : words)
			unit_test::AssertEqual(t.Search(w), true, " TrieTree: search elements which it tree");

		t.Remove("bye");
		unit_test::AssertEqual(t.Search("by"), true, "Trie Tree: serach word: by");
		unit_test::AssertEqual(t.Search("bye"), false, "Trie Tree: serach word: bye");
		t.Remove("a");
		unit_test::AssertEqual(t.Search("any"), true, "Trie Tree: serach word: any");
		unit_test::AssertEqual(t.Search("a"), false, "Trie Tree: serach word: a");
		t.Remove("lol");
		unit_test::AssertEqual(t.Search("loh"), true, "Trie Tree: serach word: loh");
		unit_test::AssertEqual(t.Search("lol"), false, "Trie Tree: serach word: lol");

		t.Insert("bye"); t.Insert("a"), t.Insert("lol");

		for (const auto & w : words)
			t.Remove(w);
		
		//unit_test::AssertEqual(t.IsEmpty(), true, "Trie Tree: remove all elements");
	}
}