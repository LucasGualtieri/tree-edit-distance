#include <iostream>
#include <format>
#include <string>
#include <filesystem>

#include "tree_generation.cpp"
#include "../data_structures/include/graph/graph.hpp"
#include "../data_structures/include/list/linearList.hpp"
#include "../data_structures/utils/Pair.hpp"

// clear && g++ -std=c++23 src/main.cpp && ./a.out

using namespace std;

const string DATA_DIR = "data";

//NOTE: Pensar num nome melhor.
//NOTE: Arrumar os valores
int table(int i) {

	switch(i) {
		case 3:
			return 30;
		case 4:
			return 30;
		case 5:
			return 30;
		case 6:
			return 30;
		case 7:
			return 30;
		case 8:
			return 30;
		case 9:
			return 30;
		case 10:
			return 30;
		case 11:
			return 30;
		case 12:
			return 30;
		default:
			return 1;
	}
}

// NOTE: Dummy Class
struct Log { void operator+=(Log a); };

Log Alg1(Graph T1, Graph T2) { return {}; }
Log Alg2(Graph T1, Graph T2) { return {}; }

void runTreeExperiment(const size_t& n, const string& name) {

	const string filePath = DATA_DIR + "/" + name;

	LinearList<Pair<Log, Log>> data;

	for (int i = 3; i <= n; i++) {

		for (int j = i; j <= n; j++) {

			Log log1, log2;

			Graph T1 = Graph::import_from_binary(format("{}/t1_2^{}_2^{}.bin", filePath, i, j));
			Graph T2 = Graph::import_from_binary(format("{}/t2_2^{}_2^{}.bin", filePath, i, j));

			int teste = min(table(i), table(j));

			for (int k = 0; k < teste; k++) {
				log1 += Alg1(T1, T2);
				log2 += Alg2(T1, T2);
			}

			data += { log1, log2 };
		}
	}

	// saveDataToCSV(data);
	// editDistance, timeTaken, memoryUsed, ...
}

//NOTE: Jogar isso pra utils.hpp
void mkdir(const std::string& folderPath) {

	if (!std::filesystem::exists(folderPath)) {
		if (!std::filesystem::create_directory(folderPath)) {
			throw runtime_error(format("Failed to create folder. \"{}\"\n", folderPath));
		}
	}
}

void generate_trees(const size_t& n, const LinearList<TreeType>& treeTypes) {

	for (TreeType type : treeTypes) {

		const string filePath = DATA_DIR + "/" + to_string(type);
		mkdir(filePath);

		for (int i = 3; i <= n; i++) {
			for (int j = i; j <= n; j++) {

				Graph T1 = generate_tree(pow(2, i), type);
				Graph T2 = generate_tree(pow(2, i), type);

				T1.export_to_binary(format("{}/t1_2^{}_2^{}.bin", filePath, i, j));
				T2.export_to_binary(format("{}/t2_2^{}_2^{}.bin", filePath, i, j));
			}
		}

		cout << format("All {} trees were successfully generated!", to_string(type)) << endl;
	}
}

int main() {
	
	size_t n = 15;

	generate_trees(n, {
		// TreeType::Binary,
		// TreeType::Linear,
		// TreeType::Star,
		// TreeType::Random,
		// TreeType::Shallow,
	});

	runTreeExperiment(n, "Binary");
	// runTreeExperiment(n, "Linear");
	// runTreeExperiment(n, "Star");
	// runTreeExperiment(n, "Random");
	// runTreeExperiment(n, "Shallow");

	return 0;
}
