#include "http_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <algorithm> // std::find
#include <vector>

namespace http {

namespace {

bool IsUsAscii(int c) {
	return static_cast<unsigned char>(c) > 127;
}

bool IsStringUsAscii(const std::string &str) {
	typedef std::string::const_iterator It;
	for (It it = str.begin(); it != str.end(); ++it) {
		if (IsUsAscii(*it)) {
			return false;
		}
	}
	return true;
}

bool IsStringUpper(const std::string &str) {
	typedef std::string::const_iterator It;
	for (It it = str.begin(); it != str.end(); ++it) {
		if (!std::isupper(static_cast<unsigned char>(*it))) {
			return false;
		}
	}
	return true;
}

} // namespace

// todo: tmp request_
HttpRequestResult HttpParse::Run(const std::string &read_buf) {
	HttpRequestResult result;
	// a: [request_line header_fields, messagebody]
	// b: [request_line, header_fields]
	std::vector<std::string> a  = utils::SplitStr(read_buf, CRLF + CRLF);
	std::vector<std::string> b  = utils::SplitStr(a[0], CRLF);
	result.request.request_line = SetRequestLine(utils::SplitStr(b[0], SP), &result.status_code);
	const std::vector<std::string> header_fields_info(b.begin() + 1, b.end());
	result.request.header_fields = SetHeaderFields(header_fields_info);
	return result;
}

RequestLine HttpParse::SetRequestLine(
	const std::vector<std::string> &request_line_info, StatusCode *status_code
) {
	RequestLine request_line;
	try {
		CheckValidRequestLine(request_line_info);
		request_line.method         = request_line_info[0];
		request_line.request_target = request_line_info[1];
		request_line.version        = request_line_info[2];
	} catch (const HttpParseException &e) {
		*status_code = e.GetStatusCode();
	}
	return request_line;
}

HeaderFields HttpParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	// todo: 各値が正常な値かどうか確認してから作成する（エラーの場合はenumに設定？）
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin(); it != header_fields_info.end(); ++it) {
		std::vector<std::string> header_key_value = utils::SplitStr(*it, ": ");
		header_fields[header_key_value[0]]        = header_key_value[1];
	}
	return header_fields;
}

void HttpParse::CheckValidRequestLine(const std::vector<std::string>& request_line_info) {
	CheckValidMethod(request_line_info[0]);
	CheckValidRequestTarget(request_line_info[1]);
	CheckValidVersion(request_line_info[2]);
}

void HttpParse::CheckValidMethod(const std::string &method) {
	// US-ASCIIかまたは大文字かどうか -> 400
	if (IsStringUsAscii(method) == false || IsStringUpper(method) == false) {
		throw HttpParseException("Error Method: BAD_REQUEST", BAD_REQUEST);
	}
	// GET, POST, DELETEかどうか ->　501
	static const std::string basic_methods[] = {"GET", "DELETE", "POST"};
	static const std::size_t methods_size = sizeof(basic_methods) / sizeof(basic_methods[0]);
	if (std::find(basic_methods, basic_methods + methods_size, method) == basic_methods + methods_size) {
		throw HttpParseException(NOT_IMPLEMENTED);
	}
}

void HttpParse::CheckValidRequestTarget(const std::string &reqest_target) {
	// /が先頭になかったら場合 -> 400
	if (reqest_target.empty() || reqest_target[0] != '/') {
		throw HttpParseException(BAD_REQUEST);
	}
}

void HttpParse::CheckValidVersion(const std::string &version) {
	// HTTP/1.1かどうか -> 400
	if (version != "HTTP/1.1") {
		throw HttpParseException(BAD_REQUEST);
	}
}

HttpParse::HttpParseException::HttpParseException(StatusCode status_code)
	: status_code_(status_code) {}

StatusCode HttpParse::HttpParseException::GetStatusCode() const {
	return status_code_;
}

// status_line && header
// messagebody

} // namespace http
