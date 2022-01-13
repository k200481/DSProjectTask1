#pragma once

#include <iostream>
#include "LinkedList.h"
#include "DSA.h"
#include "Queue.h"
#include "Stack.h"

template <typename V>
class Graph
{
public:
	// a single edge between two vertices
	struct Edge
	{
		const size_t src_idx;
		const size_t dst_idx;
		float weight;
	};

public:
	//
	Graph() = default;
	// adds a new vertex to the graph
	void AddVertex(const V& val)
	{
		assert(!HasVertex(val) && "Attempted to add duplicate vertices");
		verts.push_back(val);
		edges.push_back(SinglyLinkedList<Edge>());
	}
	// creates an undirected edge b/w given vertices
	void AddEdge(const V& src, const V& dst, float weight = 0.0f)
	{
		const size_t src_idx = GetVertIdx(src);
		const size_t dst_idx = GetVertIdx(dst);

		AddEdge_idx(src_idx, dst_idx, weight);
	}
	// creates an undirected edge b/w vertices at given indices
	void AddEdge_idx(size_t src_idx, size_t dst_idx, float weight = 0.0f)
	{
		assert(src_idx < verts.size() && "Vertex does not exist");
		assert(dst_idx < verts.size() && "Vertex does not exist");
		edges[src_idx].push_back({ src_idx, dst_idx, weight });
	}
	
	// performs bredth first search on graph starting at the source node
	// until the dst node is found, returns path from source to dst
	// returns the shortest path
	DSA<V> BFS(const V& src, const V& dst) const
	{
		const size_t src_idx = GetVertIdx(src);
		const size_t dst_idx = GetVertIdx(dst);
		
		const auto& indices = BFS_idx(src_idx, dst_idx);
		DSA<V> path(indices.size());
		for (size_t i = 0; i < indices.size(); i++)
		{
			path[i] = verts[indices[i]];
		}
		return path;
	}
	// performs bredth first search on graph starting at the source idx
	// until the dst node is found, returns path from source to dst
	// returns the shortest path in terms of vertex indices
	DSA<size_t> BFS_idx(size_t src_idx, size_t dst_idx) const
	{
		assert(src_idx < verts.size() && "Vertex does not exist");
		assert(dst_idx < verts.size() && "Vertex does not exist");

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
				return path;
			}

			// iterate over all vertices adjacent to the last vertex in the path
			for (auto& e : edges[path.back()])
			{
				// if a vertex doesn't form a cycle add it to the path
				// and push the new path onto the queue
				if (!path.Has(e.dst_idx))
				{
					auto temp = path;
					temp.push_back(e.dst_idx);
					q.push(temp);
				}
			}
		}

		return DSA<size_t>();
	}

	// performs depth first search on graph starting at the given source node
	// until the dst node is found, returns path from source to dst
	// will most likely NOT return the shortest path
	DSA<V> DFS(const V& src, const V& dst) const
	{
		const size_t src_idx = GetVertIdx(src);
		const size_t dst_idx = GetVertIdx(dst);

		const auto& indices = DFS_idx(src_idx, dst_idx);
		DSA<V> path(indices.size());
		for (size_t i = 0; i < indices.size(); i++)
		{
			path[i] = verts[indices[i]];
		}
		return path;
	}
	// performs depth first search on graph starting at the given source node
	// until the dst node is found, returns path from source to dst
	// will most likely NOT return the shortest path (in terms of vtx indices)
	DSA<size_t> DFS_idx(size_t src_idx, size_t dst_idx) const
	{
		assert(src_idx < verts.size() && "Vertex does not exist");
		assert(dst_idx < verts.size() && "Vertex does not exist");

		// stack for dfs
		LinkedListStack<DSA<size_t>> s;
		// push first list onto stack
		s.push(DSA<size_t>{ 1, src_idx });

		while (!s.empty())
		{
			// get first path in stack
			auto path = s.top();
			s.pop();

			if (path.back() == dst_idx)
			{
				return path;
			}

			// iterate over all vertices adjacent to the last vertex in the path
			for (auto& e : edges[path.back()])
			{
				// if a vertex doesn't form a cycle add it to the path
				// and push the new path onto the stack
				if (!path.Has(e.dst_idx))
				{
					auto temp = path;
					temp.push_back(e.dst_idx);
					s.push(temp);
				}
			}
		}

		return DSA<size_t>();
	}
	
	// returns all vertices stored in the graph
	const DSA<V>& GetVertices() const
	{
		return verts;
	}
	// returns all vertices stored in the graph (non-const version)
	DSA<V>& GetVertices()
	{
		return verts;
	}
	// returns adj list of given vertex
	const SinglyLinkedList<Edge>& GetAdjList(const V& val) const
	{
		return GetAdjList_idx(GetVertIdx(val));
	}
	// returns adj list of given vertex (non-const version)
	SinglyLinkedList<Edge>& GetAdjList(const V& val)
	{
		return GetAdjList_idx(GetVertIdx(val));
	}
	// returns adj list of vertex at given idx
	const SinglyLinkedList<Edge>& GetAdjList_idx(size_t idx) const
	{
		return edges[idx];
	}
	// returns adj list of vertex at given idx (non-const version)
	SinglyLinkedList<Edge>& GetAdjList_idx(size_t idx)
	{
		return edges[idx];
	}

	// returs the index of a given vertex
	// if vertex does not exist, returns the size of the vertex array
	size_t GetVertIdx(const V& val) const
	{
		for (size_t i = 0; i < verts.size(); i++)
		{
			if (val == verts[i])
				return i;
		}
		return verts.size();
	}
	// check if a vertex already exists
	bool HasVertex(const V& val)
	{
		return verts.Has(val);
	}

private:
	DSA<V> verts;
	DSA<SinglyLinkedList<Edge>> edges;
};

