#include <iostream>
#include "character.hpp"

void OutputHeaderField(void) {
	std::cout << "field-name"
			  << ":" << SP << "field-value" << SP << CR << LF;
}

void OutputHeaderFields(void) {
	OutputHeaderField();
	OutputHeaderField();
	OutputHeaderField();
}
