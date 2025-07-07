#include <iostream>

#include <format>
#include <string>

#include "../include/log.hpp"
#include "../include/utils.hpp"
#include "../include/heuristic.hpp"
#include "../include/progress_bar.hpp"
#include "../include/zhang_shasha.hpp"
#include "../include/tree_generation.hpp"
#include "../cpp-datastructures/graph/graph.hpp"
// #include "../cpp-datastructures/utils/pair.hpp"
// #include "../cpp-datastructures/list/linear_list.hpp"

// clear && g++ -std=c++23 src/main.cpp && ./a.out

using namespace std;

const string DATA_DIR = "data";

void generate_trees(const size_t&, const LinearList<TreeType>&);

int repetitionCount(const int& i) {

	static const std::unordered_map<int, int> repetitionMap = {
		{3, 30},
		{4, 30},
		{5, 10},
		{6, 10},
		{7, 8},
		{8, 5},
		{9, 5},
		{10, 1},
		{11, 1},
		{12, 1}
	};

	auto it = repetitionMap.find(i);
	if (it != repetitionMap.end()) return it->second;

	return 1; // default para casos não mapeados
}

void runTreeExperiment(const size_t& n, const TreeType& type) {

	const string filePath = DATA_DIR + "/" + to_string(type);

	const size_t totalPairs = (n - 2) * (n - 1) / 2;

	LinearList<Pair<Log, Log>> data;
	size_t step = 0;

	for (int i = 3; i <= n; i++) {

		for (int j = i; j <= n; j++) {

			Log log1, log2;

			Graph T1 = Graph::import_from_binary(format("{}/t1_2^{}_2^{}.bin", filePath, i, j));
			Graph T2 = Graph::import_from_binary(format("{}/t2_2^{}_2^{}.bin", filePath, i, j));

			int numRepetitions = min(repetitionCount(i), repetitionCount(j));

			for (int k = 0; k < numRepetitions; k++) {
				log1 += ZhangShasha(T1, T2);
				log2 += Heuristic(T1, T2);
				progressBar(step, 0, totalPairs, format("i: {}, j: {}, k: {}/{}", i, j, k, numRepetitions));
			}

			step++;

			data += { log1, log2 };
		}
	}

	progressBar(step, 0, totalPairs);

	exportToCSV(data, format("results/raw/{}_data.csv", to_string(type)));
	plot_TED_heatmap(to_string(type));
	plot_TIME_heatmap(to_string(type));
}

int main() {

	size_t n = 10;

	generate_trees(n, {
		// TreeType::Binary,
		// TreeType::Linear,
		// TreeType::Star,
		// TreeType::Random,
		// TreeType::Shallow,
	});

	runTreeExperiment(n, TreeType::Binary);
	// runTreeExperiment(n, TreeType::Star);
	// runTreeExperiment(n, TreeType::Random);
	// runTreeExperiment(n, TreeType::Shallow);
	// runTreeExperiment(n, TreeType::Linear);

	return 0;
}

void generate_trees(const size_t& n, const LinearList<TreeType>& treeTypes) {

	for (TreeType type : treeTypes) {

		const string filePath = DATA_DIR + "/" + to_string(type);
		mkdir(filePath);

		for (int i = 3; i <= n; i++) {
			for (int j = i; j <= n; j++) {

				Graph T1 = generate_tree(pow(2, i), type);
				Graph T2 = generate_tree(pow(2, j), type);

				T1.export_to_binary(format("{}/t1_2^{}_2^{}.bin", filePath, i, j));
				T2.export_to_binary(format("{}/t2_2^{}_2^{}.bin", filePath, i, j));
			}
		}

		cout << format("All {} trees were successfully generated!", to_string(type)) << endl;
	}
}
