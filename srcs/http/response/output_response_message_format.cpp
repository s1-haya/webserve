#include "character.hpp"
#include <iostream>
#include <map>
#include "http.hpp"

void                                       OutputHeaderFields(std::ostream &response_stream);

void OutputStatusLine(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << "HTTP/1.1" << SP << request.at(Http::HTTP_STATUS) << SP
			  << request.at(Http::HTTP_STATUS_TEXT) << CR << LF;
}

void OutputCRLF(std::ostream &response_stream) {
	response_stream << CR << LF;
}

void OutputBody(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << request.at(Http::HTTP_CONTENT) << std::endl;
}

void OutputResponse(std::ostream &response_stream, const Http::RequestMessage &request) {
	OutputStatusLine(response_stream, request);
	OutputHeaderFields(response_stream);
	OutputCRLF(response_stream);
	OutputBody(response_stream, request);
}
