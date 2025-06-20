#include <iostream>

#include <format>
#include <string>

#include "tree_generation.cpp"

#include "../include/log.hpp"
#include "../include/utils.hpp"
#include "../include/heuristic.hpp"
#include "../include/progress_bar.hpp"
#include "../include/zhang_shasha.hpp"
#include "../external/cpp-datastructures/include/graph/graph.hpp"
#include "../external/cpp-datastructures/include/list/linear_list.hpp"

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
		{10, 3},
		{11, 2},
		{12, 2}
	};

	auto it = repetitionMap.find(i);
	if (it != repetitionMap.end()) return it->second;

	return 1; // default para casos não mapeados
}

void runTreeExperiment(const size_t& n, const TreeType& type) {

	const string filePath = DATA_DIR + "/" + to_string(type);

	const size_t totalPairs = (n - 2) * (n - 1) / 2;

	vector<pair<Log, Log>> data;
	data.reserve(totalPairs);   // optional but avoids reallocations
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
			}

			log1.avg(numRepetitions);
			log2.avg(numRepetitions);

			progressBar(step++, 0, totalPairs, format("i: {}, j: {}, k: {}", i, j, numRepetitions));
			data.push_back({ log1, log2 });
		}
	}

	progressBar(step, 0, totalPairs);

	for (auto [l1, l2] : data) {

		cout << format("l1: {:.3f}s, TED: {}, T1: {}, T2: {}", l1.duration_secs, l1.edit_distance, l1.T1, l1.T2) << endl;
		cout << format("l2: {:.3f}s, TED: {}, T1: {}, T2: {}", l2.duration_secs, l2.edit_distance, l2.T1, l2.T2) << endl;
		cout << "-------------------------" << endl;
	}

	// saveDataToCSV(data);
	// editDistance, timeTaken, memoryUsed, ...
}

int main() {

	size_t n = 3;

	generate_trees(n, {
		// TreeType::Binary,
		// TreeType::Linear,
		// TreeType::Star,
		// TreeType::Random,
		// TreeType::Shallow,
	});

	runTreeExperiment(n, TreeType::Binary);
	// runTreeExperiment(n, TreeType::Linear);
	// runTreeExperiment(n, TreeType::Star);
	// runTreeExperiment(n, TreeType::Random);
	// runTreeExperiment(n, TreeType::Shallow);

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
