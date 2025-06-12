#ifndef UNION_FIND
#define UNION_FIND

#include <unordered_map>

#include "../include/list/linearList.hpp"

template <typename T>
class UnionFind {

	std::unordered_map<T, T> parent;
	std::unordered_map<T, int> rank;

	size_t sets = 0;

  public:

	size_t numberOfSets() { return sets; }

	void insert(T vertex) {
		parent[vertex] = vertex;
		rank[vertex] = 0;
		sets++;
	}

	bool connected(T u, T v) { return find(u) == find(v); }

	T find(T vertex) {

		if (parent[vertex] != vertex) {
			parent[vertex] = find(parent[vertex]); // Path compression
		}

		return parent[vertex];
	}

	void join(T u, T v) {

		T rootU = find(u), rootV = find(v);

		if (rootU == rootV) return;

		sets--;

		if (rank[rootU] > rank[rootV]) {
			parent[rootV] = rootU;
		}

		else if (rank[rootU] < rank[rootV]) {
			parent[rootU] = rootV;
		}

		else {
			parent[rootV] = rootU;
			rank[rootU]++;
		}
	}

	LinearList<T> heads() {

		LinearList<T> heads;

		for (const auto& [a, b] : parent) {
			if (a == b) heads += a;
		}

		return heads;
	}
};

#endif
