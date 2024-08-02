#include "http_response.hpp"
#include "http_message.hpp"
#include <iostream>
#include <sstream>

namespace http {

// HttpResponseResult構造体をHttpResponseのフォーマットを文字列に出力する
std::string HttpResponse::CreateHttpResponse(const HttpResponseResult &response) {
	std::ostringstream response_stream;
	CreateStatusLine(response_stream, response.status_line);
	CreateHeaderFields(response_stream, response.header_fields);
	response_stream << CRLF;
	CreateBodyMessage(response_stream, response.body_message);
	return response_stream.str();
}

void HttpResponse::CreateStatusLine(std::ostream &response_stream, const StatusLine &status_line) {
	response_stream << status_line.version << SP << status_line.status_code << SP
					<< status_line.status_reason << CRLF;
}

void HttpResponse::CreateHeaderField(
	std::ostream &response_stream, const std::string &name, const std::string &value
) {
	response_stream << name << ":" << SP << value << CRLF;
}

void HttpResponse::CreateHeaderFields(
	std::ostream &response_stream, const HeaderFields &header_fields
) {
	typedef HeaderFields::const_iterator It;
	for (It it = header_fields.begin(); it != header_fields.end(); it++) {
		CreateHeaderField(response_stream, it->first, it->second);
	}
}

void HttpResponse::CreateBodyMessage(
	std::ostream &response_stream, const std::string &body_message
) {
	response_stream << body_message;
}

} // namespace http
