#include "character.hpp"
#include <iostream>
#include "http.hpp"

void CreateBody(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << request.at(Http::HTTP_CONTENT) << std::endl;
}
