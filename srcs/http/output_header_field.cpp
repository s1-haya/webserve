#include "character.hpp"
#include <iostream>

void OutputHeaderField(void) {
	std::cout << "field-name"
			  << ":" << SP << "field-value" << SP << CR << LF;
}

void OutputHeaderFields(void) {
	OutputHeaderField();
	OutputHeaderField();
	OutputHeaderField();
}

// int main(void) {
// 	OutputHeaderField();
// 	return 0;
// }
