#ifndef TREE_GENERATION_HPP
#define TREE_GENERATION_HPP

#include <random>
#include <cmath>
#include <unordered_map>

#include "../cpp-datastructures/graph/graph.hpp"
#include "../cpp-datastructures/graph/graph_builder.hpp"

enum class TreeType { Binary, Linear, Star, Random, Shallow };

inline std::string to_string(const TreeType& type) {

	const static std::unordered_map<TreeType, std::string> table = {
		{ TreeType::Binary,	 "Binary" },
		{ TreeType::Linear,	 "Linear" },
		{ TreeType::Star,	 "Star" },
		{ TreeType::Random,	 "Random" },
		{ TreeType::Shallow, "Shallow" },
	};

	return table.at(type);
}

inline Graph generate_tree(size_t n, TreeType type, Graph::DataStructures ds = Graph::FastAdjacencyList) {

	using namespace std;

	Graph tree = GraphBuilder()
		// NOTE: Isso é interessante, mas exige mudança nas estruturas
		// .vertices(n)
		// .edges(n - 1)
		.directed()
		.dataStructure(ds)
	.build();

	mt19937 rng(random_device{}());

	for (int i = 0; i < n; i++) {
		tree.addVertex(i);
	}

	switch (type) {

		case TreeType::Binary: {

			for (size_t i = 1; i < n; i++) {
				size_t parent = (i - 1) / 2;
				tree.addEdge(parent, i);
			}

			break;
		}

		case TreeType::Linear: {

			for (size_t i = 1; i < n; i++) {
				tree.addEdge(i - 1, i);
			}

			break;
		}

		case TreeType::Star: {

			for (size_t i = 1; i < n; i++) {
				tree.addEdge(0, i);
			}

			break;
		}

		case TreeType::Random: {

			for (size_t i = 1; i < n; i++) {

				uniform_int_distribution<size_t> dist(0, i - 1);
				size_t parent = dist(rng);
				tree.addEdge(parent, i);
			}

			break;
		}

		case TreeType::Shallow: {

			size_t depth = max(size_t(2), size_t(log2(n / 2)));
			size_t per_level = ceil(n / static_cast<double>(depth));
			size_t current = 1;

			for (size_t d = 0; d < depth && current < n; d++) {

				size_t parent_start = d == 0 ? 0 : current - per_level;

				for (size_t i = 0; i < per_level && current < n; i++) {

					size_t parent = parent_start + (i % (current - parent_start));
					tree.addEdge(parent, current);
					current++;
				}
			}

			break;
		}
	}

	return tree;
}

#endif
