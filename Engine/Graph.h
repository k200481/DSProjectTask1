#pragma once

#include <iostream>
#include "LinkedList.h"
#include "DSA.h"
#include "Queue.h"
#include "Stack.h"

template <typename T>
class Graph
{
public:
	struct Edge
	{
		size_t src;
		size_t dst;
		int weight;
	};

public:
	Graph() = default;
	// adds a new vertex to the graph
	void AddVertex(const T& val)
	{
		if (HasVertex(val))
		{
			throw std::exception("Attempted to add duplicate vertices");
		}
		verts.push_back(val);
		edges.push_back(SinglyLinkedList<Edge>());
	}
	// creates an undirected edge b/w given vertices
	// if vertices do not exist, creates them
	void AddEdge(const T& src, const T& dst, int weight = 1)
	{
		size_t verts_size = verts.size();
		size_t src_idx = GetVertIdx(src);
		size_t dst_idx = GetVertIdx(dst);

		if (src_idx == verts_size)
		{
			AddVertex(src);
			src_idx = verts.size() - 1;
		}
		if (dst_idx == verts_size)
		{
			AddVertex(dst);
			dst_idx = verts.size() - 1;
		}

		edges[src_idx].push_back({ src_idx, dst_idx, weight });
		//edges[dst_idx].push_back({ dst_idx, src_idx, weight });
	}
	// performs bredth first search on graph starting at the source node
	// until the dst node is found, returns path from source to dst
	// returns the shortest path
	DSA<T> BFS(const T& src, const T& dst) const
	{
		auto src_idx = GetVertIdx(src);
		if (src_idx == verts.size())
		{
			throw std::exception("Vertex does not exist");
		}
		auto dst_idx = GetVertIdx(dst);
		if (dst_idx == verts.size())
		{
			throw std::exception("Vertex does not exist");
		}

		// queue for bfs
		LinkedListQueue<DSA<size_t>> q;
		// push first list onto queue
		q.push(DSA<size_t>{ 1, src_idx });

		while (!q.empty())
		{
			// get first path in queue
			auto path = q.front();
			q.pop();

			if (path.back() == dst_idx)
			{
				DSA<T> ans;
				for (auto& idx : path)
				{
					ans.push_back(verts[idx]);
				}
				return ans;
			}

			// iterate over all vertices adjacent to the last vertex in the path
			for (auto& e : edges[path.back()])
			{
				// if a vertex doesn't form a cycle add it to the path
				// and push the new path onto the queue
				if (!path.Has(e.dst))
				{
					auto temp = path;
					temp.push_back(e.dst);
					q.push(temp);
				}
			}
		}

		return DSA<T>();
	}
	// performs depth first search on graph starting at the given source node
	// until the dst node is found, returns path from source to dst
	// will most likely NOT return the shortest path
	DSA<T> DFS(const T& src, const T& dst) const
	{
		size_t src_idx = GetVertIdx(src);
		if (src_idx == verts.size())
		{
			throw std::exception("Src vertex does not exist");
		}
		size_t dst_idx = GetVertIdx(dst);
		if (dst_idx == verts.size())
		{
			throw std::exception("Dst vertex does not exist");
		}

		// stack for DFS
		LinkedListStack<DSA<size_t>> s;
		// push first path onto stack
		s.push(DSA<size_t>{ 1, src_idx });

		while (!s.empty())
		{
			// get the path at the top of the stack
			auto f = s.top();
			s.pop();

			if (f.back() == dst_idx)
			{
				DSA<T> path;
				for (auto& idx : f)
				{
					path.push_back(verts[idx]);
				}
				return path;
			}

			// iterate over all vertices adjacent to the last vertex in the path
			for (auto& e : edges[f.back()])
			{
				// if a vertex doesn't form a cycle add it to the path
				// and push the new path onto the queue
				if (!f.Has(e.dst))
				{
					auto temp = f;
					temp.push_back(e.dst);
					s.push(temp);
				}
			}
		}

		return DSA<T>();
	}
	// runs the bellman ford algorithm on the graph to find the 
	// shortest path to every node from the given source node
	void Bellmanford(const T& src) const
	{
		size_t src_idx = GetVertIdx(src);
		size_t num_verts = verts.size();

		DSA<int> dist(num_verts, INT_MAX);
		dist[src_idx] = 0;

		for (size_t i = 0; i < num_verts - 1; i++)
		{
			// equivalent to a single for loop through all edges
			for (auto& lst : edges)
			{
				for (auto& e : lst)
				{
					if (dist[e.src] != INT_MAX && dist[e.dst] > dist[e.src] + e.weight)
					{
						dist[e.dst] = dist[e.src] + e.weight;
					}
				}
			}
		}

		// check for negative weight cycle
		for (auto& lst : edges)
		{
			for (auto& e : lst)
			{
				if (dist[e.src] != INT_MAX && dist[e.dst] > dist[e.src] + e.weight)
				{
					throw std::exception("Graph has negative weight cycle\n");
				}
			}
		}

		for (size_t i = 0; i < verts.size(); i++)
		{
			std::cout << "dist(" << verts[i] << ") = " << dist[i] << std::endl;
		}
	}
	// performs topological sort to produce a topological ordering of the graph
	void TopSort() const
	{
		DSA<bool> visited(verts.size(), false);
		LinkedListStack<size_t> ans;
		
		size_t idx = 0;
		while (true)
		{
			TopSortHelper(idx, ans, visited);
			bool completed = true;
			for (size_t i = 0; i < verts.size(); i++)
			{
				if (!visited[i])
				{
					idx = i;
					completed = false;
				}
			}

			if (completed)
				break;
		}

		while (!ans.empty())
		{
			std::cout << verts[ans.top()] << ' ';
			ans.pop();
		}
	}
	// detects any cycle within the graph
	bool IsCyclic() const
	{
		LinkedListQueue<DSA<size_t>> paths;

		for (size_t i = 0; i < verts.size(); i++)
		{
			DSA<size_t> path;
			path.push_back(i);

			paths.push(path);
			while (!paths.empty())
			{
				auto front = paths.front();
				paths.pop();

				auto last = front.back();
				for (auto& e : edges[last])
				{
					if (front.Has(e.dst))
					{
						return true;
					}

					auto new_path = front;
					new_path.push_back(e.dst);
					paths.push(new_path);
				}
			}
		}
		return false;
	}
	// quite literally just bellman ford in a loop
	void FloydWarshall()
	{
		for (auto& elem : verts)
		{
			std::cout << "With " << elem << " as source\n";
			Bellmanford(elem);
		}
	}
	
	// check if a vertex already exists
	bool HasVertex(const T& val)
	{
		return verts.Has(val);
	}
	const DSA<T>& GetVertices() const
	{
		return verts;
	}
	const SinglyLinkedList<Edge>& GetAdjList(const T& val) const
	{
		return edges[GetVertIdx(val)];
	}

private:
	// recursive DFS for TopSort
	void TopSortHelper(size_t idx, LinkedListStack<size_t>& ans, DSA<bool>& visited) const
	{
		visited[idx] = true;
		for (const auto& e : edges[idx])
		{
			if (!visited[e.dst])
				TopSortHelper(e.dst, ans, visited);
		}
		ans.push(idx);
	}
	// returs the index of a given vertex
	// if vertex does not exist, returns the size of the vertex array
	size_t GetVertIdx(const T& val) const
	{
		for (size_t i = 0; i < verts.size(); i++)
		{
			if (val == verts[i])
				return i;
		}
		return verts.size();
	}

private:
	DSA<T> verts;
	DSA<SinglyLinkedList<Edge>> edges;
};

