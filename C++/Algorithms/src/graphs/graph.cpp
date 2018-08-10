#include"graph.h"
#include"../unit_test/unit_test.h"

#include<algorithm>
#include<iterator>

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

	map<size_t, vector<int>> Graph::ConnsectedCompNumb()
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
		copy(begin(res), end(res), ostream_iterator<int>(cout, " "));
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
}