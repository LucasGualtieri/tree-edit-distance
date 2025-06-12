#ifndef LOG_HPP
#define LOG_HPP

#include "timer.hpp"
#include <stdexcept>
#include <tuple>
#include <fstream>
#include <format>
#include "../DataStructures/include/list/linearList.hpp"

class Log {
	
	Timer timer;
	LinearList<std::tuple<size_t, size_t, float, double>> log;

  public:

	void registrar(const size_t& n, const size_t& m, const float& density) {
		log += { n, m, density, timer.result() };
	}

	void startTimer() { timer.start(); }
	void stopTimer() { timer.stop(); }

	void save(const std::string& filename) {

		std::ofstream outFile("data/" + filename + ".csv");

		if (!outFile) {
			throw std::runtime_error(format("Error: Could not open the file \"{}\" for writing.\n", filename));
		}

		outFile << "n, m, density, time\n";

		for (const auto& t : log) {
			outFile << std::format("{}, {}, {}, {:.3f}\n", get<0>(t), get<1>(t), get<2>(t), get<3>(t));
		}

		std::cout << format("The {}.csv file's been saved", filename) << std::endl;

		outFile.close();
	}
};

#endif
