#include"graph.h"
#include"../unit_test/unit_test.h"

#include<algorithm>
#include<iterator>
#include<queue>

//#include<iostream>

namespace graph
{
	Graph::Graph(bool is_oriented) : is_oriented_(is_oriented), vertex_count_(0) { }

	void Graph::Add(int start, int end)
	{
		AddHelper(start, end);

		if (!is_oriented_)
			AddHelper(end, start);
	}

	bool Graph::Find(int start, int end)
	{
		auto it = body_.find(start);
		return (it == body_.end()) ? false : (it->second.find(end) != it->second.end());
	}

	size_t Graph::GetVertexNumb() const
	{
		return body_.size();
	}

	void Graph::DFS()
	{
		// Mark all vertex as not visited
		unordered_map<int, bool> is_visited;
		for (const auto & p : body_)
			is_visited.insert({ p.first, false });

		for (const auto & p : body_)
		{
			// Visit all not visited vertexes and their neighbours
			if (!is_visited[p.first])
				Explore(is_visited, p.first);
		}
	}

	map<size_t, vector<int>> Graph::ConnectedCompNumb()
	{
		// Mark all vertex as not visited
		unordered_map<int, bool> is_visited;
		for (const auto & p : body_)
			is_visited.insert({ p.first, false });
		// Stores component of con
		unordered_map<int, size_t> cc_num;
		size_t cur_cc_id = 0;

		for (const auto & p : body_)
		{
			// Visit all not visited vertexes and their neighbours
			if (!is_visited[p.first])
				Explore(is_visited, p.first, cc_num, cur_cc_id);
			++cur_cc_id;
		}

		// Construct result map : cc_comp_id : nodes in this component
		map <size_t, vector<int>> res;
		for_each(begin(cc_num), end(cc_num), [&res](const pair<int, size_t> & pair)
		{
			auto it = res.find(pair.second);
			if (it == res.end())
				res.insert({ pair.second, {pair.first} });
			else
				it->second.push_back(pair.first);
		});

		return res;
	}

	map<size_t, vector<int>> Graph::StrongConnectedCompNumb()
	{
		// Create and fill transpoce graph
		Graph gr_transpoce(is_oriented_);

		for (const auto & p : body_)
			for (const auto & neigb_id : p.second)
				gr_transpoce.Add(neigb_id, p.first);

		// Obtain Topological sort of transpose graph
		auto gr_transpoce_top_sort = gr_transpoce.TopologicalSortDFS();
		copy(begin(gr_transpoce_top_sort), end(gr_transpoce_top_sort), ostream_iterator<int>(cout, " "));
		cout << endl;

		// Do DFS for our (NOT TRANSPOCED) graph in order of topological sort of
		// transpose graph, obtained previously
		unordered_map<int, bool> is_visited;
		for (const auto & p : body_)
			is_visited.insert({ p.first, false });
		unordered_map<int, size_t> cc_numb;
		size_t cur_numb = 0;

		for (auto it = rbegin(gr_transpoce_top_sort); it != rend(gr_transpoce_top_sort); ++it)
		{
			if (!is_visited[*it])
				Explore(is_visited, *it, cc_numb, cur_numb);
			++cur_numb;
		}

		// Construct result
		map<size_t, vector<int>> res;
		for_each(begin(cc_numb), end(cc_numb), [&res](const pair<int, size_t> & pair)
		{
			auto it = res.find(pair.second);
			if (it == res.end())
				res.insert({ pair.second,{ pair.first } });
			else
				it->second.push_back(pair.first);
		});

		return res;
	}

	bool Graph::IsCircleDetect()
	{
		// Here we loop intil we find the FIRST circle
		unordered_map<int, bool> is_visited;
		for (const auto & p : body_)
			is_visited.insert({ p.first, false });

		// We could simplify this procedure and instead of wok_time allocate
		// memory on each Explore() call and check if it is zero but here I
		// implement algorithm in accordeace with CS explanation
		unordered_map<int, pair<size_t, size_t>> work_time;
		bool is_circle = false;  size_t cur_time = 0;

		for (const auto & p : body_)
			if (!is_circle && !is_visited[p.first])
				ExploreHelper(is_visited, p.first, work_time, cur_time, is_circle);

		return is_circle;
	}

	void Graph::TopoligicalSort()
	{
		if (IsCircleDetect())
		{
			cout << "Circle is detected: Topoligical sort is not possible." << endl;
			return;
		}

		// Fill array which stores number of input links to current node of graph
		unordered_map<int, int> outs;
		for (const auto & p : body_)
			outs.insert({ p.first, 0 });

		for_each(begin(body_), end(body_), [&outs](const pair<int, set<int>> & p)
		{
			for (const auto & v : p.second)
				++outs[v];
		});

		// Find node with has NO incoming links [Always possible to 
		// find such a node in graph without circles]
		auto it = find_if(begin(outs), end(outs), [](const pair<int, int> & p) -> bool
		{
			return p.second == 0;
		});

		deque<int> d = { it->first };
		vector<int> res = { it->first };
		outs.erase(it);

		while (!d.empty())
		{
			int vertex_id = *begin(d); d.pop_front(); outs.erase(vertex_id);

			// Obtain number of incoming to nodes links after remove node[vertex_id]
			for (auto & neighb_id : body_.at(vertex_id))
				--outs[neighb_id];
			
			// Fill dequae with new nodes which has no incoming links,
			// and in the same time remove them from outs 
			for (auto it = begin(outs); it != end(outs);)
			{
				if (it->second == 0)
				{
					d.push_back(it->first);
					res.push_back(it->first);
					it = outs.erase(it);
				}
				else
					++it;
			}
		}

		// Display result of topoligical sorting
		copy(begin(res), end(res), ostream_iterator<int>(cout, ", "));
	}

	vector<int> Graph::TopologicalSortDFS()
	{
		vector<int> result;
		unordered_map<int, bool> is_visited;
		for (const auto & p : body_)
			is_visited.insert({ p.first, false });

		map<int, set<int>> b(begin(body_), end(body_));

		for (const auto & p : b)
			if (!is_visited[p.first])
				ExploreHelperTopSort(is_visited, p.first, result);

		return result;
	}

	void Graph::BFS(size_t start_id)
	{
		vector<int> dist(GetVertexNumb(), INT_MAX);
		dist[start_id] = 0;

		queue<size_t> d;
		d.push(start_id);

		while (!d.empty())
		{
			size_t cur_id = d.front(); d.pop();
			for (const auto & neighb_id : body_[cur_id])
				if (dist[neighb_id] == INT_MAX)
				{
					dist[neighb_id] = dist[cur_id] + 1;
					d.push(neighb_id);
				}
		}

		for (size_t i = 0; i < dist.size(); ++i)
			cout << start_id << " -> " << i << " = " << dist[i] << endl;
	}

	vector<size_t> Graph::ShortestDistance(size_t start_id, size_t end_id)
	{
		vector<int> dist(GetVertexNumb(), INT_MAX);
		dist[start_id] = 0;
		// Stores previous element (on level top in path)
		vector<size_t> prev_el(GetVertexNumb(), 0);
		
		queue<size_t> d;
		d.push(start_id);

		// BFS algorithm
		while (!d.empty())
		{
			size_t cur_id = d.front(); d.pop();
			for (const auto & neighb_id : body_[cur_id])
				if (dist[neighb_id] == INT_MAX)
				{
					dist[neighb_id] = dist[cur_id] + 1;
					prev_el[neighb_id] = cur_id;
					d.push(neighb_id);
				}
		}


		if (dist[end_id] = INT_MAX)
		{
			cout << "Could not go from " << start_id << " vertex to " << end_id << " vertex." << endl;
			return vector<size_t>();
		}
		else
		{
			// Construct path using prev_el array
			list<size_t> shortest_path;
			size_t cur_id = end_id;
			while (cur_id != start_id)
			{
				shortest_path.push_front(cur_id);
				cur_id = prev_el[cur_id];
			}
			shortest_path.push_front(cur_id);

			return vector<size_t>(begin(shortest_path), end(shortest_path));
		}
	}

	void Graph::AddHelper(int start, int end)
	{
		auto ins_pos = body_.find(start);

		if (ins_pos == body_.end())
			body_.insert({ start,{ end } });
		else
			ins_pos->second.insert(end);
	}

	void Graph::Explore(unordered_map<int, bool>& is_visited, int vertex_id)
	{
		//cout << vertex_id << " ";
		is_visited[vertex_id] = true;

		for (const auto & neighb_id : body_[vertex_id])
			if (!is_visited[neighb_id])
				Explore(is_visited, neighb_id);
	}

	void Graph::Explore(unordered_map<int, bool>& is_visited, int vertex_id, 
		unordered_map<int, size_t>& cc_numb, size_t cur_cc_id)
	{
		is_visited[vertex_id] = true;
		cc_numb.insert({ vertex_id, cur_cc_id });

		for (const auto & neighb_id : body_[vertex_id])
			if (!is_visited[neighb_id])
				Explore(is_visited, neighb_id, cc_numb, cur_cc_id);
	}

	void Graph::ExploreHelper(unordered_map<int, bool>& is_visited, int vertex_id,
		unordered_map<int, pair<size_t, size_t>>& times, size_t & cur_time, bool & is_circle)
	{
		is_visited[vertex_id] = true;
		times.insert({ vertex_id, {cur_time, 0} });

		for (const auto & neighb_id : body_[vertex_id])
		{
			auto it = times.find(neighb_id);
			if (it != times.end() && it->second.second == 0)
			{
				is_circle = true;
				return;
			}

			if (!is_circle && !is_visited[neighb_id])
				ExploreHelper(is_visited, neighb_id, times, ++cur_time, is_circle);
		}

		times[vertex_id].second = ++cur_time;
		// If we whant topological sorting output
		cout << vertex_id << " ";
	}

	void Graph::ExploreHelperTopSort(unordered_map<int, bool>& is_visited, int vertex_id, vector<int>& top_sort_out)
	{
		is_visited[vertex_id] = true;

		for (const auto & neighb_id : body_[vertex_id])
			if (!is_visited[neighb_id])
				ExploreHelperTopSort(is_visited, neighb_id, top_sort_out);

		top_sort_out.push_back(vertex_id);
	}

	ostream & operator<<(ostream & os, const Graph & gr)
	{
		for_each(begin(gr.body_), end(gr.body_), [&os](const pair<int, set<int>> & v)
		{
			os << v.first << " : ";
			copy(begin(v.second), end(v.second), ostream_iterator<int>(os, " "));
			os << endl;
		});

		return os;
	}

	bool operator<(const WeightNode & left, const WeightNode & right)
	{
		return left.end_id < right.end_id;
	}

	bool operator==(const WeightNode & left, const WeightNode & right)
	{
		return left.end_id == right.end_id;
	}

	ostream & operator<<(ostream & os, const WeightNode & wn)
	{
		os << wn.end_id << " (" << wn.w << ")";
		return os;
	}

	ostream & operator<<(ostream & os, const GraphWeight & wg)
	{
		for_each(begin(wg.body_), end(wg.body_), [&os](const pair<size_t, set<WeightNode>> & p)
		{
			os << p.first << " : ";
			copy(begin(p.second), end(p.second), ostream_iterator<WeightNode>(os, " "));
			os << endl;
		});
		return os;
	}

	void GraphWeight::Add(size_t start_id, size_t end_id, int weight)
	{
		auto it = body_.find(start_id);

		if (it == body_.end())
			body_.insert({ start_id, {{end_id, weight}} });
		else
			it->second.insert({ end_id, weight });
	}

	vector<size_t> GraphWeight::ChipestPath(size_t start_id, size_t end_id)
	{
		vector<int> dist(body_.size(), INT_MAX);
		vector<size_t> prev(body_.size(), 0);

		DijkstraHelper(start_id, dist, prev);

		if (dist[end_id] != INT_MAX)
		{
			list<size_t> path;
			size_t cur_id = end_id;
			while (cur_id != start_id)
			{
				path.push_front(cur_id);
				cur_id = prev[cur_id];
			}
			path.push_front(start_id);

			return vector<size_t>(begin(path), end(path));
		}

		return vector<size_t>();
	}

	int GraphWeight::ChipestPathCost(size_t start_id, size_t end_id)
	{
		vector<int> dist(body_.size(), INT_MAX);
		vector<size_t> prev(body_.size(), 0);

		DijkstraHelper(start_id, dist, prev);

		return dist[end_id];
	}

	void GraphWeight::DijkstraHelper(size_t start_id, vector<int> & dist, vector<size_t> & prev)
	{
		//vector<int> dist(body_.size(), INT_MAX);
		//vector<size_t> prev(body_.size(), 0);

		dist[start_id] = 0;

		vector<pair<int, size_t>> pq(body_.size(), pair<int, size_t>());
		for (const auto p : body_)
			pq[p.first] = { dist[p.first], p.first };

		make_heap(begin(pq), end(pq), [](const pair<int, size_t> & left, const pair<int, size_t> & right)
		{
			return left.first > right.first;
		});

		while (!pq.empty())
		{
			size_t cur_id = begin(pq)->second; 
			pop_heap(begin(pq), end(pq), [](const pair<int, size_t> & left, const pair<int, size_t> & right)
			{
				return left.first > right.first;
			});
			pq.pop_back();

			for (const auto & cur_neigb : body_[cur_id])
			{
				size_t neigb_id = cur_neigb.end_id;
				int cur_w = GetWeight(cur_id, neigb_id);

				if (dist[neigb_id] > dist[cur_id] + cur_w)
				{
					dist[neigb_id] = dist[cur_id] + cur_w;
					prev[neigb_id] = cur_id;
				}
				// Update priority because distance has been changed
				make_heap(begin(pq), end(pq), [](const pair<int, size_t> & left, const pair<int, size_t> & right)
				{
					return left.first > right.first;
				});
			}
		}

		/*for(auto it = begin(dist); it != end(dist); ++it)
		{
			cout << start_id << " -> " << distance(begin(dist), it) << ": d = " << *it << endl;
		}*/
	}

	int GraphWeight::GetWeight(size_t start_id, size_t end_id)
	{
		auto it = body_.find(start_id);

		if (it != body_.end())
		{
			auto pos = find_if(begin(it->second), end(it->second), [&end_id](const WeightNode & wn)
			{
				return wn.end_id == end_id;
			});

			return (pos == end(it->second)) ? 0 : pos->w;
		}

		return 0;
	}

}