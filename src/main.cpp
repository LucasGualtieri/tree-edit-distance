#include <iostream>
#include <format>
#include <string>
#include <filesystem>

#include "tree_generation.cpp"
#include "../data_structures/include/graph/graph.hpp"
#include "../data_structures/include/list/linearList.hpp"

using namespace std;

//NOTE: Pensar num nome melhor
const string PATH = "data";

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

void runTreeExperiment(const size_t& n, const string& name) {

	const string filePath = PATH + "/" + name;

	// LinerList<Pair<Log, Log>> data;

	for (int i = 3; i <= n; i++) {

		for (int j = i; j <= n; j++) {

			Graph T1 = Graph::import_from_binary(format("{}/t1_2^{}_2^{}.bin", filePath, i, j));
			Graph T2 = Graph::import_from_binary(format("{}/t2_2^{}_2^{}.bin", filePath, i, j));

			int teste = min(table(i), table(j));

			for (int k = 0; teste; k++) {
				// log1 += Alg1(t1, t2);
				// log2 += Alg2(t1, t2);
			}

			// data += { log1, log2 };
		}
	}

	// savaData(data);
}

//NOTE: Jogar isso pra utils.hpp
void mkdir(const std::string& folderPath) {

	if (!std::filesystem::exists(folderPath)) {
		if (!std::filesystem::create_directory(folderPath)) {
			throw runtime_error(format("Failed to create folder. \"{}\"\n", folderPath));
		}
	}
}

void foo(const size_t& n,const LinearList<TreeType>& list) {

	for (TreeType t : list) {

		const string filePath = PATH + "/" + to_string(t);
		mkdir(filePath);

		for (int i = 3; i <= n; i++) {
			for (int j = i; j <= n; j++) {

				Graph T1 = generate_tree(pow(2, i), t);
				Graph T2 = generate_tree(pow(2, i), t);

				T1.export_to_binary(format("/{}t1_2^{}_2^{}.bin", filePath, i, j));
				T2.export_to_binary(format("/{}t2_2^{}_2^{}.bin", filePath, i, j));
			}
		}
	}
}

int main() {
	
	size_t n = 15;

	foo(n, {
		TreeType::Binary,
		TreeType::Linear,
		TreeType::Star,
		TreeType::Random,
		TreeType::Shallow
	});

	// runTreeExperiment(n, "binary");
	// runTreeExperiment(n, "random");

	return 0;
}
