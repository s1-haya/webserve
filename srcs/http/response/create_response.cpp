#include "http.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include <sstream>

namespace http {
namespace {

void CreateStatusLine(std::ostream &response_stream, Http::RequestMessage &request) {
	response_stream << HTTP_VERSION << SP << request[Http::HTTP_STATUS] << SP
					<< request[Http::HTTP_STATUS_TEXT] << CRLF;
}

template <typename T>
void CreateHeaderField(std::ostream &response_stream, const std::string &name, const T &value) {
	response_stream << name << ":" << SP << value << SP << CRLF;
}

void CreateHeaderFields(std::ostream &response_stream, Http::RequestMessage &request) {
	CreateHeaderField(response_stream, CONNECTION, "close");
	CreateHeaderField(response_stream, CONTENT_LENGTH, request[Http::HTTP_CONTENT].size());
}

void CreateCRLF(std::ostream &response_stream) {
	response_stream << CRLF;
}

void CreateBody(std::ostream &response_stream, Http::RequestMessage &request) {
	response_stream << request[Http::HTTP_CONTENT];
}

} // namespace

HttpResult Http::CreateResponse() {
	std::ostringstream response_stream;
	CreateStatusLine(response_stream, this->request_);
	CreateHeaderFields(response_stream, this->request_);
	CreateCRLF(response_stream);
	CreateBody(response_stream, this->request_);

	HttpResult result;
	result.is_response_complete = true;
	result.response             = response_stream.str();
	return result;
}

} // namespace http
