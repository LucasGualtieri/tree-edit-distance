#include <random>
#include <cmath>

#include "../data_structures/include/graph/graph.hpp"
#include "../data_structures/include/graph/graph_builder.hpp"

enum class TreeType { Binary, Linear, Star, Random, Shallow };

std::string to_string(TreeType type) {

	switch (type) {
		case TreeType::Binary:
			return "Binary";
		case TreeType::Linear:
			return "Linear";
		case TreeType::Star:
			return "Star";
		case TreeType::Random:
			return "Random";
		case TreeType::Shallow:
			return "Shallow";
		default:
			return "Unknown";
	}
}

Graph generate_tree(size_t n, TreeType type, Graph::DataStructures ds = Graph::AdjacencyList) {

	using namespace std;

	Graph tree = GraphBuilder()
		.vertices(n)
		.edges(n - 1)
		.directed()
		.dataStructure(ds)
		.build();

	mt19937 rng(random_device{}());

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
