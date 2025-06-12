#ifndef DATA_STRUCTURE_HPP
#define DATA_STRUCTURE_HPP

#include "../list/linearList.hpp"
#include "../../utils/Pair.hpp"

#include "Edge.hpp"
#include <stdexcept>

class DataStructure {

	public:

	virtual void addEdge(const Vertex& u, const Vertex& v, float weight = 1.0) {
		throw std::runtime_error("addEdge: Method not implemented!");
	}

	virtual void removeEdge(const Vertex& u, const Vertex& v) {
		throw std::runtime_error("removeEdge: Method not implemented!");
	}

	virtual void changeEdgeWeight(const Edge& e, const float& weight) {
		throw std::runtime_error("changeEdgeWeight: Method not implemented!");
	}
 
	virtual Edge getEdge(const Edge& e) const {
		throw std::runtime_error("getEdge: Method not implemented!");
	}
 
	virtual void addVertex(const Vertex& v) {
		throw std::runtime_error("addVertex: Method not implemented!");
	}
 
	virtual size_t degree(const Vertex& v) const {
		throw std::runtime_error("degree: Method not implemented!");
	}
 
	virtual bool hasEdge(const Vertex& u, const Vertex& v, const float& weight) const {
		throw std::runtime_error("hasEdge: Method not implemented!");
	}
 
	virtual bool hasEdge(const Vertex& u, const Vertex& v) const {
		throw std::runtime_error("hasEdge: Method not implemented!");
	}
 
	virtual bool hasVertex(const Vertex& v) const {
		throw std::runtime_error("hasVertex: Method not implemented!");
	}
 
	virtual void weighted(bool _weighted) {
		throw std::runtime_error("weighted: Method not implemented!");
	}
 
	virtual void directed(bool _directed) {
		throw std::runtime_error("directed: Method not implemented!");
	}
 
	virtual LinearList<Edge> edges() const {
		throw std::runtime_error("Edge: Method not implemented!");
	}
 
	virtual LinearList<Vertex> vertices() const {
		throw std::runtime_error("vertices: Method not implemented!");
	}
 
	virtual LinearList<Pair<Vertex, float>> neighbors(const Vertex& u) const {
		throw std::runtime_error("neighbors: Method not implemented!");
	}
 
	virtual void print() {
		throw std::runtime_error("print: Method not implemented!");
	}
 
	virtual ~DataStructure() {}
};

#endif
