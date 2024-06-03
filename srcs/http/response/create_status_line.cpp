#include "character.hpp"
#include <iostream>
#include "http.hpp"

void CreateStatusLine(
	std::ostream &response_stream, const Http::RequestMessage &request
) {
	response_stream << "HTTP/1.1" << SP << request.at(Http::HTTP_STATUS) << SP
					<< request.at(Http::HTTP_STATUS_TEXT) << CR << LF;
}
