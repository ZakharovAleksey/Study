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
		// Removement done !!! But ont complited -> because did not deallocate full memory !!!
		// Play with nodes_ptr to deal with this situation
		void Remove(const string & word);

		bool IsEmpty() const;
	private:

		bool DeleteHelper(TrieNodePtr cur_node, const string & word, string::const_iterator cur_symb_it);

	private:
		TrieNodePtr root_;
	};


	void TrieTreeTest();
}

#endif // !PREFIX_TREE_H