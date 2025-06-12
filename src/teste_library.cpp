#include <iostream>

#include "../external/cpp-datastructures/include/list/linear_list.hpp"

// clear && g++ teste_library.cpp -std=c++23 && ./a.out

int main() {

	LinearList<int> teste = { 1, 2, 3, 4 };

	std::cout << teste << std::endl;

	return 0;
}
