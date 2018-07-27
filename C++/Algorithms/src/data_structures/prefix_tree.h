#ifndef PREFIX_TREE_H
#define PREFIX_TREE_H

#pragma once

#include<unordered_map>
#include<memory>

namespace tree
{
	using namespace std;

	struct TrieNode;
	using  TrieNodePtr = shared_ptr<TrieNode>;

	struct TrieNode
	{
		unordered_map<char, TrieNodePtr> body;
		bool is_complete_word;

		TrieNode() : body(), is_complete_word(false) {}
	};

	// Trie Tree: 
	// https://www.geeksforgeeks.org/?p=13067
	class TrieTree
	{
	public:
		TrieTree();

		void Insert(const string & word);
		bool Search(const string & word);
		void Remove(const string & word);

	private:

		// Not finished -> deal with removing
		tuple<bool, bool, bool, bool> Find(const string & word);

	private:
		TrieNodePtr root_;
	};


}

#endif // !PREFIX_TREE_H