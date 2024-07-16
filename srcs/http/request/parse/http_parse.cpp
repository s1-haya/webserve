#include "http_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <algorithm> // std::find
#include <vector>

namespace http {

namespace {

bool IsUSASCII(const std::string &str) {
	typedef std::string::const_iterator It;
	for (It it = str.begin(); it != str.end(); it++) {
		if (static_cast<unsigned char>(*it) > 127)
			return false;
	}
	return true;
}

bool IsUpper(const std::string &str) {
	typedef std::string::const_iterator It;
	for (It it = str.begin(); it != str.end(); it++) {
		if (!std::isupper(static_cast<unsigned char>(*it)))
			return false;
	}
	return true;
}

static std::vector<std::string> CreateBasicMethods() {
	std::vector<std::string> basic_methods;
	basic_methods.push_back("GET");
	basic_methods.push_back("POST");
	basic_methods.push_back("DELETE");
	return basic_methods;
}

} // namespace

// todo: tmp request_
HttpRequestResult HttpParse::Run(const std::string &read_buf) {
	HttpRequestResult result;
	// a: [request_line header_fields, messagebody]
	// b: [request_line, header_fields]
	std::vector<std::string> a   = utils::SplitStr(read_buf, CRLF + CRLF);
	std::vector<std::string> b   = utils::SplitStr(a[0], CRLF);
	result.request.request_line  = SetRequestLine(utils::SplitStr(b[0], SP), &result.status_code);
	result.request.header_fields = SetHeaderFields(b);
	// PrintLines(b);
	return result;
}

RequestLine HttpParse::SetRequestLine(
	const std::vector<std::string> &request_line_info, StatusCode *status_code
) {
	RequestLine request_line;
	try {
		request_line.method         = CheckMethod(request_line_info[0]);
		request_line.request_target = CheckRequestTarget(request_line_info[1]);
		request_line.version        = CheckVersion(request_line_info[2]);
	} catch (const HttpParseException &e) {
		*status_code = e.GetStatusCode();
	}
	return request_line;
}

HeaderFields HttpParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	// todo: 各値が正常な値かどうか確認してから作成する（エラーの場合はenumに設定？）
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin() + 1; it != header_fields_info.end(); ++it) {
		std::vector<std::string> header_key_value = utils::SplitStr(*it, ": ");
		header_fields[header_key_value[0]]        = header_key_value[1];
	}
	return header_fields;
}

std::string HttpParse::CheckMethod(const std::string &method) {
	// US-ASCIIかまたは大文字かどうか -> 400
	if (IsUSASCII(method) == false || IsUpper(method) == false)
		throw HttpParseException(BAD_REQUEST);
	// GET, POST, DELETEかどうか ->　501
	static const std::vector<std::string> basic_methods = CreateBasicMethods();
	if (std::find(basic_methods.begin(), basic_methods.end(), method) == basic_methods.end())
		throw HttpParseException(NOT_IMPLEMENTED);
	return method;
}

std::string HttpParse::CheckRequestTarget(const std::string &reqest_target) {
	// /が先頭になかったら場合 -> 400
	if (reqest_target.empty() || reqest_target[0] != '/')
		throw HttpParseException(BAD_REQUEST);
	return reqest_target;
}

std::string HttpParse::CheckVersion(const std::string &version) {
	// HTTP/1.1かどうか -> 400
	if ("HTTP/1.1" != version)
		throw HttpParseException(BAD_REQUEST);
	return version;
}

HttpParse::HttpParseException::HttpParseException(StatusCode status_code)
	: status_code_(status_code) {}

StatusCode HttpParse::HttpParseException::GetStatusCode() const {
	return status_code_;
}

// status_line && header
// messagebody

} // namespace http
