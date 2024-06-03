#include "character.hpp"
#include <iostream>
#include <map>
#include "http.hpp"

void                                       CreateHeaderFields(std::ostream &response_stream);

void CreateStatusLine(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << "HTTP/1.1" << SP << request.at(Http::HTTP_STATUS) << SP
			  << request.at(Http::HTTP_STATUS_TEXT) << CR << LF;
}

void CreateCRLF(std::ostream &response_stream) {
	response_stream << CR << LF;
}

void CreateBody(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << request.at(Http::HTTP_CONTENT) << std::endl;
}

void CreateResponse(std::ostream &response_stream, const Http::RequestMessage &request) {
	CreateStatusLine(response_stream, request);
	CreateHeaderFields(response_stream);
	CreateCRLF(response_stream);
	CreateBody(response_stream, request);
}
