#include "http_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <vector>

namespace http {
namespace {

// debug:
// void PrintLines(const std::vector<std::string> &lines) {
// 	typedef std::vector<std::string>::const_iterator It;
// 	size_t                                           i = 0;
// 	for (It it = lines.begin(); it != lines.end(); ++it) {
// 		// if (*it == "") {
// 		// 	std::cout << "stop!" << std::endl;
// 		// 	break;
// 		// }
// 		std::cout << i << ":" << *it << std::endl;
// 		i++;
// 	}
// }

} // namespace

HttpParse::HttpParse() {}

HttpParse::~HttpParse() {}

RequestLine HttpParse::SetRequestLine(const std::vector<std::string> &request_line_info) {
	// todo 各値が正常な値かどうか確認してから作成する（エラーの場合はenumに設定？）
	RequestLine request_line;
	request_line.method  = request_line_info[0];
	request_line.uri     = request_line_info[1];
	request_line.version = request_line_info[2];
	return request_line;
}

HeaderFields HttpParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	// todo 各値が正常な値かどうか確認してから作成する（エラーの場合はenumに設定？）
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin() + 1; it != header_fields_info.end(); ++it) {
		std::vector<std::string> header_key_value = utils::SplitStr(*it, ": ");
		header_fields[header_key_value[0]]        = header_key_value[1];
	}
	return header_fields;
}

// todo: tmp request_
HttpRequest HttpParse::Run(const std::string &read_buf) {
	HttpRequest              request;
	// a: [request_line header_fields, messagebody]
	// b: [request_line, header_fields]
	std::vector<std::string> a = utils::SplitStr(read_buf, CRLF + CRLF);
	std::vector<std::string> b = utils::SplitStr(a[0], CRLF);
	request.status_line        = SetRequestLine(utils::SplitStr(b[0], SP));
	request.header_fields      = SetHeaderFields(b);
	// PrintLines(b);
	return request;
}

} // namespace http
