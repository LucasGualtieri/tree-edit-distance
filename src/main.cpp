#include <iostream>
#include <format>
#include <string>
#include <filesystem>

#include "tree_generation.cpp"
#include "../external/cpp-datastructures/include/utils/pair.hpp"
#include "../external/cpp-datastructures/include/graph/graph.hpp"
#include "../external/cpp-datastructures/include/list/linear_list.hpp"

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
struct Log {

    void operator+=(Log a) {
		
	}

    bool operator==(const Log& other) const { return true; }

    bool operator<(const Log& other) const { return true; }

    bool operator>(const Log& other) const {
        return !(*this < other) && !(*this == other);
    }
};

Log Alg1(Graph T1, Graph T2) { return {}; }
Log Alg2(Graph T1, Graph T2) { return {}; }

void runTreeExperiment(const size_t& n, const TreeType& type) {

	const string filePath = DATA_DIR + "/" + to_string(type);

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

	runTreeExperiment(n, TreeType::Binary);
	// runTreeExperiment(n, TreeType::Linear);
	// runTreeExperiment(n, TreeType::Star);
	// runTreeExperiment(n, TreeType::Random);
	// runTreeExperiment(n, TreeType::Shallow);

	return 0;
}
