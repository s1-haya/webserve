#include "convert.hpp" // use ToString()
#include "debug.hpp"   // tmp to do
#include "http.hpp"
#include "http_message.hpp"
#include <iostream>
#include <sstream>

namespace http_response {
	void CreateStatusLine(
		std::ostream &response_stream, const Http::RequestMessage &request
	) {
		response_stream << HTTP_VERSION << SP << request.at(Http::HTTP_STATUS) << SP
						<< request.at(Http::HTTP_STATUS_TEXT) << CRLF;
	}

	void CreateHeaderField(
		std::ostream      &response_stream,
		const std::string &name,
		const std::string &value
	) {
		response_stream << name << ":" << SP << value << SP << CRLF;
	}

	void CreateHeaderFields(
		std::ostream &response_stream, const Http::RequestMessage &request
	) {
		CreateHeaderField(response_stream, CONNECTION, "close");
		CreateHeaderField(
			response_stream,
			CONTENT_LENGTH,
			ToString(request.at(Http::HTTP_CONTENT).size())
		);
	}

	void CreateCRLF(std::ostream &response_stream) {
		response_stream << CRLF;
	}

	void
	CreateBody(std::ostream &response_stream, const Http::RequestMessage &request) {
		response_stream << request.at(Http::HTTP_CONTENT);
	}
} // namespace http_response

std::string Http::CreateResponse() {
	std::ostringstream response_stream;
	http_response::CreateStatusLine(response_stream, this->request_);
	http_response::CreateHeaderFields(response_stream, this->request_);
	http_response::CreateCRLF(response_stream);
	http_response::CreateBody(response_stream, this->request_);
	return response_stream.str();
}
