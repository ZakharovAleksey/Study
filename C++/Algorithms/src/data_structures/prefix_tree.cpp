#include"prefix_tree.h"

#include<iostream>
#include<algorithm>

namespace tree
{
	TrieTree::TrieTree() : root_(new TrieNode()) {}

	void TrieTree::Insert(const string & word)
	{
		TrieNodePtr cur_node = root_;
		for(auto symb_it = begin(word); symb_it != end(word); ++symb_it)
		{
			cur_node->body.insert({ *symb_it, make_shared<TrieNode>() });
			cur_node = cur_node->body[*symb_it];
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
				cur_node = next_pos->second;
			else
				return false;
		}
		return (cur_node->is_complete_word);
	}

	void TrieTree::Remove(const string & word)
	{
		bool is_in_tree = false, is_unique_key = false, is_contains_prefix = false, is_prefix_itself = false;
		tie(is_in_tree, is_unique_key, is_contains_prefix, is_prefix_itself) = Find(word);

		if (!is_in_tree)
			return;
		
		
		// If it is unique word (has no other prefix words, or is not prefix itself)
		
	}

	tuple<bool, bool, bool, bool> TrieTree::Find(const string & word)
	{
		TrieNodePtr cur_node = root_;
		bool is_in_tree = false, 
			 is_unique_key = true, // Тут надо еще проверить на то что у нас в дереве одно слово a
			 is_contains_prefix = false,
			 is_prefix_itself = false;

		for (auto symb_it = begin(word); symb_it != end(word); ++symb_it)
		{
			auto next_pos = find_if(begin(cur_node->body), end(cur_node->body), [&symb_it](const pair<char, TrieNodePtr> & p)
			{
				return p.first == *symb_it;
			});

			// Check on unique key: In: red, redmont, lol -> remove lol
			// If on road sizes (from SECOND to the end) of all hash_map's are equal to 1 - this is unique key
			if (distance(begin(word), symb_it) == 1 && cur_node->body.size() == 1)
				is_unique_key = true;

			// Check if this key contains prefix: In: red, redmont, lol, lolol -> remove lolol
			// If on the the road we obtain even one is_complete_word true  - this key contains prefix
			if (cur_node->is_complete_word)
				is_contains_prefix = true;

			if (next_pos != end(cur_node->body))
				cur_node = next_pos->second;
			// Check if this key is not found in tree: In: red, redmont -> remove lol
			else
				return tie(is_in_tree, is_unique_key, is_contains_prefix, is_prefix_itself);
		}

		// Check if this key is prefix for another word:
		// If on the next layer for last letter in word we have not empty node: In: red, redmont, lol, lolol -> remove lol
		if (!cur_node->body[*rbegin(word)]->body.empty())
			is_prefix_itself = true;

		// Еще рассмотреть случай lol loa [a, l - different letters]

		if (cur_node->is_complete_word)
			is_in_tree = true;

		return tie(is_in_tree, is_unique_key, is_contains_prefix, is_prefix_itself);
	}

}