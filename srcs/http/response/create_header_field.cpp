#include "character.hpp"
#include <iostream>
#include "http.hpp"

void CreateHeaderField(std::ostream &response_stream) {
	response_stream << "field-name"
					<< ":" << SP << "field-value" << SP << CR << LF;
}

void CreateHeaderFields(std::ostream &response_stream) {
	CreateHeaderField(response_stream);
	CreateHeaderField(response_stream);
	CreateHeaderField(response_stream);
}
