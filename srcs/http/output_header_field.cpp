#include <iostream>
#include "character.hpp"

void OutputHeaderField(void) {
	std::cout << "field-name"
			  << ":" << SP << "field-value" << SP << CR << LF;
}

int main(void) {
	OutputHeaderField();
	return 0;
}
