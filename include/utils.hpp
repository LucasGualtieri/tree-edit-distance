#ifndef UTILS_HPP
#define UTILS_HPP

#include "log.hpp"
#include <string>
#include <format>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iomanip> // for std::fixed and std::setprecision

inline void mkdir(const std::string& folderPath) {

	if (!std::filesystem::exists(folderPath)) {
		if (!std::filesystem::create_directory(folderPath)) {
			throw std::runtime_error(format("Failed to create folder. \"{}\"\n", folderPath));
		}
	}
}

inline void clearscreen() {
    #ifdef _WIN32
		std::system("cls");
    #else
        std::system("clear");
    #endif
}

inline void exportToCSV(const std::vector<std::pair<Log, Log>>& data, const std::string& filename) {

    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing.");
    }

    // Write header
    file << "T1,T2,time_taken_t1,time_taken_t2,edit_distance_t1,edit_distance_t2\n";

    for (const auto& [log1, log2] : data) {
        file << log1.T1 << ',' << log1.T2 << ',' 
			<< std::fixed << std::setprecision(4) << log1.duration_secs << ','
			<< std::fixed << std::setprecision(4) << log2.duration_secs << ','
			<< log1.edit_distance << ',' << log2.edit_distance << '\n';
    }

    file.close();
}

inline void plot_TED_heatmap(const std::string& tree_type) {

    std::string command = std::format("python3 scripts/plot_edit_distance_diff.py {}", tree_type);
    int result = std::system(command.c_str());
    if (result != 0) {
        throw std::runtime_error(std::format("Erro ao executar o script para tipo de árvore: {}", tree_type));
    }
}

inline void plot_TIME_heatmap(const std::string& tree_type) {

    std::string command = std::format("python3 scripts/plot_time_diff.py {}", tree_type);
    int result = std::system(command.c_str());
    if (result != 0) {
        throw std::runtime_error(std::format("Erro ao executar o script para tipo de árvore: {}", tree_type));
    }
}

#endif
