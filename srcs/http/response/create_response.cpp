#include "character.hpp"
#include "http.hpp"
#include <iostream>
#include <sstream>

namespace http_response {
	void CreateStatusLine(
		std::ostream &response_stream, const Http::RequestMessage &request
	) {
		response_stream << "HTTP/1.1" << SP << request.at(Http::HTTP_STATUS) << SP
						<< request.at(Http::HTTP_STATUS_TEXT) << CR << LF;
	}

	void CreateHeaderField(std::ostream &response_stream) {
		response_stream << "field-name"
						<< ":" << SP << "field-value" << SP << CR << LF;
	}

	void CreateHeaderFields(std::ostream &response_stream) {
		CreateHeaderField(response_stream);
		CreateHeaderField(response_stream);
		CreateHeaderField(response_stream);
	}

	void CreateCRLF(std::ostream &response_stream) {
		response_stream << CR << LF;
	}

	void
	CreateBody(std::ostream &response_stream, const Http::RequestMessage &request) {
		response_stream << request.at(Http::HTTP_CONTENT) << std::endl;
	}
} // namespace http_response

const std::string Http::CreateResponse() {
	std::ostringstream response_stream;
	http_response::CreateStatusLine(response_stream, this->request_);
	http_response::CreateHeaderFields(response_stream);
	http_response::CreateCRLF(response_stream);
	http_response::CreateBody(response_stream, this->request_);
	return response_stream.str();
}
