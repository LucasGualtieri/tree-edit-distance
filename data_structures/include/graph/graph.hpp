#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "../list/linearList.hpp"
#include "../../utils/Pair.hpp"

#include "AdjacencyMatrix.hpp"
#include "FastAdjacencyList.hpp"
#include "AdjacencyMatrixPointers.hpp"
#include "AdjacencyList.hpp"
#include "DataStructure.hpp"
#include "Edge.hpp"
#include "FastAdjacencyList.hpp"
#include <cmath>
#include <cstdint>
#include <fstream>
#include <string>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <format>
#include <unordered_map>

// Custom hash specialization for std::pair
namespace std {
	template <>
	struct hash<Pair<Vertex, Vertex>> {
		size_t operator()(const Pair<Vertex, Vertex>& p) const {
			return hash<int>()(p.first) ^ (hash<int>()(p.second) << 1);
		}
	};
}

class Graph {

public:

	enum DataStructures {
		AdjacencyMatrix, AdjacencyMatrixPointers,
		AdjacencyList, FastAdjacencyList
	};

	using Prop = Pair<std::string, std::string>;

private:

	Pair<std::string, std::string> base = {"", ""};

	class EdgeMap : public std::unordered_map<Pair<Vertex, Vertex>, Pair<std::string, std::string>> {
		Pair<std::string, std::string> base = {"", ""};
	public:
		Pair<std::string, std::string> Find(const Pair<Vertex, Vertex>& edge) const {
			auto it = find(edge);
			return it != end() ? it->second : base;
		}
	};

	class VertexMap : public std::unordered_map<Vertex, Pair<std::string, std::string>> {

		Pair<std::string, std::string> base = {"", ""};

	  public:

		Pair<std::string, std::string> Find(const Vertex& v) const {
			auto it = find(v);
			return it != end() ? it->second : base;
		}
	};

	class VertexLabel : public std::unordered_map<Vertex, std::string> {

		std::string base = "";

	  public:

		std::string Find(const Vertex& v) const {
			auto it = find(v);
			return it != end() ? it->second : base;
		}
	};

	DataStructure* dataStructure;
	bool directed, weighted;
	DataStructures choice;
	EdgeMap edgeMap;
	VertexMap vertexMap;
	VertexLabel vertexLabel;

	Graph(DataStructure* dataStructure, size_t n, bool directed, bool weighted) {

		this->directed = directed;
		this->weighted = weighted;
		this->n = n;
		this->dataStructure = dataStructure;
	}

	DataStructure* dataStructureChoice(DataStructures choice) {

		switch (choice) {
			case AdjacencyMatrix:
				return new class AdjacencyMatrix(n);
			case AdjacencyMatrixPointers:
				return new class AdjacencyMatrixPointers(n);
			case AdjacencyList:
				return new class AdjacencyList;
			case FastAdjacencyList:
				return new class FastAdjacencyList;
			break;
		}

		return nullptr;
	}

public:

	size_t n, m;

	Graph(DataStructures choice, size_t n, size_t m, bool directed, bool weighted) {

		this->directed = directed;
		this->weighted = weighted;
		this->choice = choice;
		this->n = n;
		this->m = m;

		dataStructure = dataStructureChoice(choice);

		this->dataStructure->directed(directed);
		this->dataStructure->weighted(weighted);
	}

	Graph (const Graph& clone) {

		// NOTE: This is the function called by the "return graph;" "context".

		directed = clone.directed;
		weighted = clone.weighted;
		choice = clone.choice;
		n = 0;
		m = 0;

		dataStructure = dataStructureChoice(choice);

		this->dataStructure->directed(directed);
		this->dataStructure->weighted(weighted);

		for (const Vertex& v : clone.vertices()) {
			addVertex(v);
		}

		for (const Edge& e : clone.edges()) {
			addEdge(e);
		}
	}

	Graph& operator=(const Graph& G) {

		if (this != &G) {

			directed = G.directed;
			weighted = G.weighted;
			choice = G.choice;
			n = G.n;
			m = G.m;

			delete dataStructure;

			dataStructure = dataStructureChoice(choice);

			dataStructure->directed(directed);
			dataStructure->weighted(weighted);

			for (const Vertex& v : G.vertices()) {
				addVertex(v);
			}

			for (const Edge& e : G.edges()) {
				addEdge(e);
			}
		}

		return *this;
	}	

	Graph cloneDataStructure(size_t n = 0) const {
		return { choice, n, m, directed, weighted };
	}

	void printDataStructure() { dataStructure->print(); }

	~Graph() { delete dataStructure; }

	void addVertex(const Vertex& v) {
		dataStructure->addVertex(v);
		n++;
	}

	void addVertex(const Vertex& v, const Pair<std::string, std::string>& props) {
		addVertex(v);
		vertexMap.insert({v, props});
	}

	void addVertex(const Vertex& v, std::string& label) {
		addVertex(v);
		vertexLabel.insert({v, label});
	}

	void addVertex(const Vertex& v, const std::string& label, const Pair<std::string, std::string>& props) {
		addVertex(v);
		vertexMap.insert({v, props});
		vertexLabel.insert({v, label});
	}

	std::string getLabel(const Vertex& v) const {
		return vertexLabel.Find(v);
	}

	void addEdge(const Vertex& u, const Vertex& v, const float& weight) {

		if (hasEdge({u, v})) return;

		if (!weighted) {
			throw std::runtime_error("You should not pass the weight of the edge");
		}

		if (!dataStructure->hasVertex(u)) addVertex(u);

		if (!dataStructure->hasVertex(v)) addVertex(v);

		dataStructure->addEdge(u, v, weight);

		if (!directed) dataStructure->addEdge(v, u, weight);

		m++;
	}

	void addEdge(const Vertex& u, const Vertex& v) {

		if (weighted) {
			throw std::runtime_error("You must pass the weight of the edge");
		}

		if (!dataStructure->hasVertex(u)) addVertex(u);

		if (!dataStructure->hasVertex(v)) addVertex(v);

		dataStructure->addEdge(u, v);

		if (!directed) dataStructure->addEdge(v, u);

		m++;
	}

	void addEdge(const Vertex& u, const Vertex& v, const Pair<std::string, std::string>& props) {
		if (hasEdge({u, v})) return;
		addEdge(u, v);
		edgeMap.insert({{u, v}, props});
	}

	void addEdge(const Edge& e) {

		if (hasEdge(e)) return;

		if (!dataStructure->hasVertex(e.u)) addVertex(e.u);

		if (!dataStructure->hasVertex(e.v)) addVertex(e.v);

		if (weighted) {

			dataStructure->addEdge(e.u, e.v, e.weight);

			if (!directed) dataStructure->addEdge(e.v, e.u, e.weight);
		}

		else {

			dataStructure->addEdge(e.u, e.v);

			if (!directed) dataStructure->addEdge(e.v, e.u);
		}

		m++;
	}

	void addEdge(const Edge& e, const Pair<std::string, std::string>& props) {
		if (hasEdge(e)) return;
		addEdge(e);
		edgeMap.insert({{e.u, e.v}, props});
	}

	void removeEdge(const Edge& e) {
		dataStructure->removeEdge(e.u, e.v);
		m--;
	}

	void changeEdgeProps(const Vertex& u, const Vertex& v, const Prop prop) {
		if (hasEdge(u, v)) {
			edgeMap[{u, v}] = prop;
		}
	}

	void changeEdgeProps(const Edge& e, const Prop prop) {
		if (hasEdge(e.u, e.v)) {
			edgeMap[{e.u, e.v}] = prop;
		}
	}

	// NOTE: There probably should be a check if there is the vertex
	void changeVertexProps(const Vertex& v, const Prop& prop) {
		vertexMap[v] = prop;
	}

	void changeVertexLabel(const Vertex& v, const std::string& label) {
		vertexLabel[v] = label;
	}

	void changeEdgeWeight(const Edge& e, const float& weight) {
		dataStructure->changeEdgeWeight(e, weight);
	}

	void changeEdgeWeight(const Vertex& u, const Vertex& v, const float& weight) {
		dataStructure->changeEdgeWeight({ u, v }, weight);
	}

	Edge getEdge(const Edge& e) const {
		return dataStructure->getEdge(e);
	}

	Edge getEdge(const Vertex& u, const Vertex& v) const {
		return dataStructure->getEdge({u, v});
	}

	bool hasEdge(const Edge& e) const {
		return dataStructure->hasEdge(e.u, e.v);
	}

	bool hasEdge(const Vertex& u, const Vertex& v) const {
		return dataStructure->hasEdge(u, v);
	}

	size_t degree(const Vertex& v) const {
		return dataStructure->degree(v);
	}

	LinearList<Edge> edges() const { return dataStructure->edges(); }

	LinearList<Vertex> vertices() const {
		return dataStructure->vertices();
	}

	LinearList<Pair<Vertex, float>> edgesOf(const Vertex& u) const {
		return dataStructure->neighbors(u);
	}

	LinearList<Vertex> neighbors(const Vertex& u) const {

		LinearList<Vertex> _neighbors;

		for (Pair<Vertex, float> p : dataStructure->neighbors(u)) {
			_neighbors.push_back(p.first);
		}

		return _neighbors;
	}

	LinearList<Pair<Vertex, float>> operator[](const Vertex& u) {
		return dataStructure->neighbors(u);
	}

	// Function to export the graph to a PNG image using Graphviz
	void export_to_png(const std::string& filename, const std::string& engine = "dot", size_t dpi = 300) const {

		// NOTE: This is possible to define all nodes and edges at once
		// node [shape=ellipse, style=filled, fillcolor=lightblue];
		// edge [color=gray, style=dotted];

		// 1. Create a DOT file representing the graph
		std::string dotFilename = filename + ".dot";
		std::ofstream dotFile(dotFilename);

		if (!dotFile) {
			throw std::runtime_error("Could not create the DOT file.");
		}

		dotFile << (directed ? "digraph" : "graph") << " G {\n";

		dotFile << std::format("    dpi = {};", dpi) << std::endl;

		// for (const auto& [v, propsObject] : vertexMap) {
		for (const Vertex& v : vertices()) {
			// auto [color, line] = propsObject;
			auto [color, line] = vertexMap.Find(v);
			std::string label = vertexLabel.Find(v) == "" ? std::format("{}", v) : vertexLabel.Find(v);
			std::string props = "color = \"" + color + "\", style = \"" + line;
			dotFile << "    " << v << " [ " << props << format("\", label=\"{}\"]", label) << ";\n";
		}

		dotFile << std::endl;

		for (const Edge& e : edges()) {

			if (!directed && e.u > e.v) continue;  // Avoid duplicate edges for undirected graphs
			
			Prop prop = edgeMap.Find({e.u, e.v});
			auto [color, line] = prop == base && !directed ? edgeMap.Find({e.v, e.u}) : prop;

			std::string props = "color = \"" + color + "\", style = \"" + line;

			dotFile << "    " << e.u << (directed ? " -> " : " -- ") << e.v << " [label=\"" << (weighted ? std::format("{:.6g}", e.weight) : "") << "\", " << props << "\"];\n";
		}

		dotFile << "}\n";
		dotFile.close();

		// 2. Use the system() function to run the Graphviz command
		std::string command = engine + " -Tpng " + dotFilename + " -o " + filename + ".png";

		if (system(command.c_str()) != 0) {
			throw std::runtime_error("Failed to execute Graphviz command.");
		}

		std::cout << "Graph exported successfully to " << filename << ".png\n";
	}

	// TODO: Separar as arestas por virgulas
	std::string str() const {

		std::stringstream os;

		for (Vertex& u : vertices()) {

			os << u << ": { ";

			for (auto& [v, w] : edgesOf(u)) {

				if (weighted) {
					os << "(" << v << ", " << w << ") ";
				}

				else os << v << " ";
			}

			os << "}" << std::endl;
		}

		std::string result = os.str();

		return result.empty() ? result : result.substr(0, result.size() - 1);
	}

	friend std::ostream& operator<<(std::ostream& os, const Graph& g) {
		os << g.str();
		return os;
	}

	double density(const int& precision = 3) const {

		double n = this->n;
		double m = this->m;

		double density = m / ((n * n - n) / (directed ? 1 : 2));

		double factor = std::pow(10, precision);
		return std::round(density * factor) / factor;
	}

	float sparsness() const { return 1 - density(); }

	void export_to_binary(const std::string& filename) const {

		std::ofstream file(filename, std::ios::binary);

		if (!file) {
			throw std::runtime_error("Não foi possível abrir o arquivo para escrita.");
		}

		uint32_t vertex_count = static_cast<uint32_t>(n);
		uint32_t edge_count = static_cast<uint32_t>(m);

		file.write(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));
		file.write(reinterpret_cast<char*>(&edge_count), sizeof(uint32_t));

		for (const Edge& e : edges()) {

			uint32_t u = static_cast<uint32_t>(e.u);
			uint32_t v = static_cast<uint32_t>(e.v);
			file.write(reinterpret_cast<char*>(&u), sizeof(uint32_t));
			file.write(reinterpret_cast<char*>(&v), sizeof(uint32_t));
		}

		file.close();
	}

	static Graph import_from_binary(const std::string& filename) {

		std::ifstream file(filename, std::ios::binary);

		if (!file) {
			throw std::runtime_error("Não foi possível abrir o arquivo para leitura.");
		}

		uint32_t vertex_count;
		uint32_t edge_count;

		file.read(reinterpret_cast<char*>(&vertex_count), sizeof(uint32_t));
		file.read(reinterpret_cast<char*>(&edge_count), sizeof(uint32_t));

		Graph::DataStructures type = Graph::AdjacencyList;
		bool directed = false, weighted = false;

		Graph G(type, vertex_count, edge_count, directed, weighted);

		for (uint32_t i = 0; i < edge_count; ++i) {

			uint32_t u, v;

			file.read(reinterpret_cast<char*>(&u), sizeof(uint32_t));
			file.read(reinterpret_cast<char*>(&v), sizeof(uint32_t));
			G.addEdge(u, v);
		}

		file.close();

		return G;
	}
};

#endif
