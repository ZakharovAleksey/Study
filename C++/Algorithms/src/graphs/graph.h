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

	// Graph with NO weights on the edges: could be oriented and not oriented
	// https://www.youtube.com/watch?v=Hy8aVfoRDW4&index=5&list=PLlb7e2G7aSpQutUr7qYIunvm04cqdr5mx
	class Graph
	{
	public:
		Graph(bool is_oriented);

		void Add(int start, int end);
		bool Find(int start, int end);
		size_t GetVertexNumb() const;

		// Algorithms based on DFS
		void DFS();
		// Calculates number of Connected Components in Graph
		map<size_t, vector<int>> ConnectedCompNumb();
		// Calculates number of STRONG Connected Components in Graph
		map<size_t, vector<int>> StrongConnectedCompNumb();

		// Finds out if Graph has got EVEN ONE circle
		bool IsCircleDetect();

		// Performs topoligicals sort of ORIENTED Graph if it is possible
		void TopoligicalSort();
		vector<int> TopologicalSortDFS();

		// Algorithms based on BFS
		void BFS(size_t start_id);
		// Finds out the shortest path between 'start_id' and 'end_id' vertexes in graph
		vector<size_t> ShortestDistance(size_t start_id, size_t end_id);

		friend ostream & operator<<(ostream & os, const Graph & gr);

	private:
		void AddHelper(int start, int end);

		void Explore(unordered_map<int, bool> & is_visited, int vertex_id);

		void Explore(unordered_map<int, bool> & is_visited, int vertex_id, 
					 unordered_map<int, size_t> & cc_numb, size_t cur_cc_id);

		void ExploreHelper(unordered_map<int, bool> & is_visited, int vertex_id,
					 unordered_map<int, pair<size_t, size_t>> & times, size_t & cur_time, 
					 bool & is_circle);

		void ExploreHelperTopSort(unordered_map<int, bool> & is_visited, int vertex_id, vector<int> & top_sort_out);

	private:
		bool is_oriented_;
		size_t vertex_count_;

		GraphBody body_;
	};

	void TestGraph();


	struct WeightNode
	{
		size_t end_id;
		int w;

		WeightNode(size_t _end_id, int weight) : end_id(_end_id), w(weight) {}

		friend bool operator<(const WeightNode & left, const WeightNode & right);
		friend bool operator==(const WeightNode & left, const WeightNode & right);

		friend ostream & operator<<(ostream & os, const WeightNode & wn);

	};

	using WeightGraphBody = unordered_map<size_t, set<WeightNode>>;


	// Graph with weights between the edges:
	// Weights could be both: positive and negative
	// https://www.youtube.com/watch?v=3kBitR8wrFU&list=PLlb7e2G7aSpQutUr7qYIunvm04cqdr5mx&index=7&t=3645s
	class GraphWeight
	{
	public:
		GraphWeight() : has_negative_edge_(false) {}

		void Add(size_t start_id, size_t end_id, int weight);

		// Calculates the chipest path in weights Grapth between 'start_id' and 'end_id' based on:
		// - If negative weights are detected: BelmanFordHelper
		// - Otherwise : DijkstraHelper
		vector<size_t> ChipestPath(size_t start_id, size_t end_id);
		// Calculates the weight of the chipest path in weights Grapth between 'start_id' and 'end_id' based on:
		// - If negative weights are detected: BelmanFordHelper
		// - Otherwise : DijkstraHelper
		int ChipestPathCost(size_t start_id, size_t end_id);

		friend ostream & operator<<(ostream & os, const GraphWeight & wg);

	private:
		int GetWeight(size_t start_id, size_t end_id);

		// Main algorithm of BFS in case of abcence of negative weights
		void DijkstraHelper(size_t start_id, vector<int> & dist, vector<size_t> & prev);
		// Main algorithm of BFS in case of presence of negative weights
		void BelmanFordHelper(size_t start_id, vector<int> & dist, vector<size_t> & prev, bool & is_negative_circle);

	private:
		WeightGraphBody body_;
		bool has_negative_edge_;
	};


	void TestWeightGraph();
}

#endif // !GRAPH_H
