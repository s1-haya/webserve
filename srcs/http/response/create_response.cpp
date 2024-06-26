#include "http.hpp"
#include "http_message.hpp"
#include <iostream>
#include <sstream>

namespace http {
namespace {

void CreateStatusLine(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << HTTP_VERSION << SP << request.at(Http::HTTP_STATUS) << SP
					<< request.at(Http::HTTP_STATUS_TEXT) << CRLF;
}

template <typename T>
void CreateHeaderField(std::ostream &response_stream, const std::string &name, const T &value) {
	response_stream << name << ":" << SP << value << SP << CRLF;
}

void CreateHeaderFields(std::ostream &response_stream, const Http::RequestMessage &request) {
	CreateHeaderField(response_stream, CONNECTION, "close");
	CreateHeaderField(response_stream, CONTENT_LENGTH, request.at(Http::HTTP_CONTENT).size());
}

void CreateCRLF(std::ostream &response_stream) {
	response_stream << CRLF;
}

void CreateBody(std::ostream &response_stream, const Http::RequestMessage &request) {
	response_stream << request.at(Http::HTTP_CONTENT);
}

} // namespace

std::string Http::CreateResponse() {
	std::ostringstream response_stream;
	CreateStatusLine(response_stream, this->request_);
	CreateHeaderFields(response_stream, this->request_);
	CreateCRLF(response_stream);
	CreateBody(response_stream, this->request_);
	return response_stream.str();
}

} // namespace http
