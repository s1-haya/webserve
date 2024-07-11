#include "http_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <vector>

namespace http {
namespace {

// todo: create path (/, /aaa, /aaa/)
std::string CreateDefaultPath(const std::string &path) {
	static const std::string location = "html";

	if (path.size() == 1) {
		return location + "/index.html";
	}
	return location + path + "/index.html";
}

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

HTTPParse::HTTPParse() {}

HTTPParse::~HTTPParse() {}

RequestLine HTTPParse::SetRequestLine(const std::vector<std::string> &request_line_info) {
	// 各値が正常な値かどうか確認してから作成する（エラーの場合はenumに設定？）
	RequestLine request_line;
	request_line.method  = request_line_info[0];
	request_line.uri     = CreateDefaultPath(request_line_info[1]);
	request_line.version = request_line_info[2];
	return request_line;
}

HeaderFields HTTPParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	// 各値が正常な値かどうか確認してから作成する（エラーの場合はenumに設定？）
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin() + 1; it != header_fields_info.end(); ++it) {
		std::vector<std::string> header_key_value = utils::SplitStr(*it, ": ");
		header_fields[header_key_value[0]]        = header_key_value[1];
	}
	return header_fields;
}

// 　懸念点：MessageBodyにCRLFが含まれていた場合 std::vectorはLineごとに持ってる
std::string HTTPParse::SetMessageBody(const std::vector<std::string> &message_body_info) {
	std::vector<std::string>::const_iterator it           = message_body_info.begin();
	std::string                              message_body = *it;
	return message_body;
}

// todo: tmp request_
HTTPRequest HTTPParse::Run(const std::string &read_buf) {
	HTTPRequest              request;
	std::vector<std::string> a = utils::SplitStr(read_buf, CRLF + CRLF);
	std::vector<std::string> b = utils::SplitStr(a[0], CRLF);
	request.status_line        = SetRequestLine(utils::SplitStr(b[0], SP));
	request.header_fields      = SetHeaderFields(b);
	// 調査: bodymessageの取得方法（ヘッダーによって仕様が変わる）
	if ("POST" == request.status_line.method)
		request.message_body = SetMessageBody(utils::SplitStr(a[1], ""));
	// PrintLines(b);
	return request;
}

// status_line && header
// messagebody

} // namespace http
