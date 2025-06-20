#ifndef ZHANG_SHASHA_HPP
#define ZHANG_SHASHA_HPP

#include <vector>
#include <stack>
#include <unordered_map>

#include "log.hpp"
#include "../external/cpp-datastructures/include/utils/timer.hpp"
#include "../external/cpp-datastructures/include/graph/graph.hpp"

struct TreeData {
	std::vector<Vertex> postorder; // postorder[i] = nó na posição i
	std::unordered_map<Vertex, int> node_to_index; // node_to_index[v] = i
	std::vector<int> leftmost; // leftmost[i] = índice do filho mais à esquerda do nó i
	std::unordered_map<int, std::vector<int>> children; // children[i] = filhos do nó i
};

inline TreeData preprocess_tree(const Graph& T, Vertex root) {

	TreeData td;
	std::stack<Pair<Vertex, bool>> stack; // {node, visited}
	std::unordered_map<Vertex, bool> visited;

	stack.push({ root, false });

	while (!stack.empty()) {

		auto [v, visited_flag] = stack.top();
		stack.pop();

		if (visited_flag) {
			td.node_to_index[v] = td.postorder.size();
			td.postorder.push_back(v);
		}

		else {

			stack.push({v, true});
			auto children = T.neighbors(v);
			td.children[v] = {};

			for (auto u : children) {
				td.children[v].push_back(u);
				stack.push({u, false});
			}
		}
	}

	// Calcula leftmost
	td.leftmost.resize(td.postorder.size());

	for (size_t i = 0; i < td.postorder.size(); ++i) {

		Vertex v = td.postorder[i];

		if (td.children[v].empty()) {
			td.leftmost[i] = i;
		}

		else {
			int first_child_idx = td.node_to_index[td.children[v][0]];
			td.leftmost[i] = td.leftmost[first_child_idx];
		}
	}

	return td;
}

inline Log ZhangShasha(const Graph& T1, const Graph& T2) {

	Timer timer;
	timer.start();

	Vertex root = 0;

	TreeData A = preprocess_tree(T1, root);
	TreeData B = preprocess_tree(T2, root);

	int m = A.postorder.size();
	int n = B.postorder.size();

	std::vector<std::vector<int>> treedist(m, std::vector<int>(n, 0));
	std::vector<std::vector<int>> forestdist(m + 1, std::vector<int>(n + 1, 0));

	// Percorrer floresta de A e B
	for (int i = 0; i < m; ++i) {

		int i_ = A.leftmost[i];

		for (int j = 0; j < n; ++j) {

			int j_ = B.leftmost[j];

			// base case for this forest
			forestdist[i_][j_] = 0;

			// Inicializa forestdist para floresta de i_..i e j_..j
			for (int di = i_; di <= i; ++di) {
				forestdist[di + 1][j_] = forestdist[di][j_] + 1;
			}

			for (int dj = j_; dj <= j; ++dj) {
				forestdist[i_][dj + 1] = forestdist[i_][dj] + 1;
			}

			for (int di = i_; di <= i; ++di) {

				for (int dj = j_; dj <= j; ++dj) {


					Vertex nodeA = A.postorder[di];
					Vertex nodeB = B.postorder[dj];

					if (A.leftmost[di] == i_ && B.leftmost[dj] == j_) {

						// Custo de renomear
						int cost = (T1.getLabel(nodeA) == T2.getLabel(nodeB)) ? 0 : 1;

						forestdist[di + 1][dj + 1] = std::min({
							forestdist[di][dj + 1] + 1,         // delete
							forestdist[di + 1][dj] + 1,         // insert
							forestdist[di][dj] + cost           // rename
						});

						treedist[di][dj] = forestdist[di + 1][dj + 1];
					}

					else {
						forestdist[di + 1][dj + 1] = std::min({
							forestdist[di][dj + 1] + 1,
							forestdist[di + 1][dj] + 1,
							forestdist[A.leftmost[di]][B.leftmost[dj]] + treedist[di][dj]
						});
					}
				}
			}
		}
	}

	timer.stop();

	return {
		.duration_secs = timer.result(),
		.edit_distance = treedist[m - 1][n - 1],
		.T1 = T1.n,
		.T2 = T2.n
	};
}

#endif
