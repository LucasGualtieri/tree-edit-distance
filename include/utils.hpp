#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <format>
#include <filesystem>

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

#endif
