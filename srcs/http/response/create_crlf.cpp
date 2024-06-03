#include "character.hpp"
#include <iostream>
#include "http.hpp"

void CreateCRLF(std::ostream &response_stream) {
	response_stream << CR << LF;
}
