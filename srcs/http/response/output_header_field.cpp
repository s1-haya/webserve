#include <iostream>
#include "character.hpp"

void OutputHeaderField(std::ostream &response_stream) {
	response_stream << "field-name"
			  << ":" << SP << "field-value" << SP << CR << LF;
}

void OutputHeaderFields(std::ostream &response_stream) {
	OutputHeaderField(response_stream);
	OutputHeaderField(response_stream);
	OutputHeaderField(response_stream);
}
