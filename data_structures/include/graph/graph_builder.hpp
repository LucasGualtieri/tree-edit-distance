#ifndef GRAPH_BUILDER_HPP
#define GRAPH_BUILDER_HPP

#include "graph.hpp"
#include <cstddef>

class GraphBuilder {

	size_t _n = 0, _m = 0;
	Graph::DataStructures choice;
	bool _directed = false, _weighted = false;

  public:

	GraphBuilder& vertices(size_t n) {

		this->_n = n;

		return *this;
	}

	GraphBuilder& edges(size_t m) {

		this->_m = m;

		return *this;
	}

	GraphBuilder& weighted() {

		_weighted = true;

		return *this;
	}

	GraphBuilder& directed() {

		_directed = true;

		return *this;
	}

	GraphBuilder& dataStructure(const Graph::DataStructures& choice) {
		this->choice = choice;
		return *this;
	}

	Graph build() { return Graph(choice, _n, _m, _directed, _weighted); }

};

#endif
