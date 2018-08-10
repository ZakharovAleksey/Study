#ifndef GRAPH_H
#define GRAPH_H

#include<unordered_map>
#include<vector>
#include<set>
#include<map>
#include<deque>

#include<fstream>

namespace graph
{

	using namespace std;
	using GraphBody = unordered_map<int, set<int>>;

	class Graph
	{
	public:
		Graph(bool is_oriented);

		void Add(int start, int end);
		bool Find(int start, int end);
		size_t GetVertexNumb() const;

		// Algorithms
		void DFS();
		map<size_t, vector<int>> ConnsectedCompNumb();
		bool IsCircleDetect();

		void TopoligicalSort();

		friend ostream & operator<<(ostream & os, const Graph & gr);

	private:
		void AddHelper(int start, int end);

		void Explore(unordered_map<int, bool> & is_visited, int vertex_id);

		void Explore(unordered_map<int, bool> & is_visited, int vertex_id, 
					 unordered_map<int, size_t> & cc_numb, size_t cur_cc_id);

		void ExploreHelper(unordered_map<int, bool> & is_visited, int vertex_id,
					 unordered_map<int, pair<size_t, size_t>> & times, size_t & cur_time, 
					 bool & is_circle);

	private:
		bool is_oriented_;
		size_t vertex_count_;

		GraphBody body_;
	};

}

#endif // !GRAPH_H
