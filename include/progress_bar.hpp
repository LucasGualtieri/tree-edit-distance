#include <format>
#include <iostream>

#include "utils.hpp"

inline void progressBar(int i, int li, int ls, const std::string& msg = "") {

	clearscreen();

	const int barWidth = 25; // Length of the progress bar
    if (i < li) i = li;
    if (i > ls) i = ls;

    // Calculate progress percentage
    double progress = ((double)(i - li) / (ls - li));
    int pos = barWidth * progress;

    // Move cursor back to the beginning of the bar
    std::cout << "\r[";

    // Print the filled portion of the bar
    for (int j = 0; j < pos; j++) std::cout << "=";

    // Print the remaining empty portion of the bar
    if (pos < barWidth) {
        std::cout << ">";
        pos++;
    }

    // Fill the rest of the bar with spaces
    for (int j = pos; j < barWidth; ++j) std::cout << " ";

    // Print the percentage
    std::cout << "] " << std::format("{:.0f}", progress * 100) << "%";
    std::cout.flush();

	std::cout << std::endl << msg << std::endl;

	if (i == ls) std::cout << std::endl;
}
