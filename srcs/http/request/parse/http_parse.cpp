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

bool IsOptionalWhitespace(char c) {
	return (c == ' ' || c == '\t');
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

// OWS (optional whitespace): SP(Space), HTAB(Horizontal Tab)
void TrimLeadingOptionalWhitespace(std::string &str) {
	typedef std::string::iterator It;
	for (It it = str.begin(); it != str.end(); ++it) {
		if (!IsOptionalWhitespace(*it)) {
			str.erase(str.begin(), it);
			return;
		}
	}
	str.erase(str.end());
}

} // namespace

// todo: tmp request_
HttpRequestResult HttpParse::Run(const std::string &read_buf) {
	HttpRequestResult result;
	// a: [request_line header_fields, messagebody]
	// b: [request_line, header_fields]
	std::vector<std::string> a = utils::SplitStr(read_buf, CRLF + CRLF);
	std::vector<std::string> b = utils::SplitStr(a[0], CRLF);
	try {
		result.request.request_line = SetRequestLine(utils::SplitStr(b[0], SP));
		const std::vector<std::string> header_fields_info(b.begin() + 1, b.end());
		result.request.header_fields = SetHeaderFields(header_fields_info);
	} catch (const HttpParseException &e) {
		result.status_code = e.GetStatusCode();
	}
	return result;
}

RequestLine HttpParse::SetRequestLine(const std::vector<std::string> &request_line_info) {
	RequestLine request_line;
	CheckValidRequestLine(request_line_info);
	request_line.method         = request_line_info[0];
	request_line.request_target = request_line_info[1];
	request_line.version        = request_line_info[2];
	return request_line;
}

HeaderFields HttpParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin(); it != header_fields_info.end(); ++it) {
		std::vector<std::string> header_key_value = utils::SplitStr(*it, ":");
		TrimLeadingOptionalWhitespace(header_key_value[1]);
		CheckValidHeaderFieldValue(header_key_value[0]);
		// 既にヘッダフィールドの値が設定されてる場合
		if (header_fields[header_key_value[0]].size()) {
			throw HttpParseException(
				"Error: The value already exists in header fields", BAD_REQUEST
			);
		}
		header_fields[header_key_value[0]] = header_key_value[1];
	}
	return header_fields;
}

void HttpParse::CheckValidRequestLine(const std::vector<std::string> &request_line_info) {
	CheckValidMethod(request_line_info[0]);
	CheckValidRequestTarget(request_line_info[1]);
	CheckValidVersion(request_line_info[2]);
}

void HttpParse::CheckValidMethod(const std::string &method) {
	// US-ASCIIかまたは大文字かどうか -> 400
	if (IsStringUsAscii(method) == false || IsStringUpper(method) == false) {
		throw HttpParseException(
			"Error: This method contains lowercase or non-USASCII characters.", BAD_REQUEST
		);
	}
	// GET, POST, DELETEかどうか ->　501
	static const std::string basic_methods[] = {"GET", "DELETE", "POST"};
	static const std::size_t methods_size    = sizeof(basic_methods) / sizeof(basic_methods[0]);
	if (std::find(basic_methods, basic_methods + methods_size, method) ==
		basic_methods + methods_size) {
		throw HttpParseException("Error: This method doesn't exist in webserv.", NOT_IMPLEMENTED);
	}
}

void HttpParse::CheckValidRequestTarget(const std::string &reqest_target) {
	// /が先頭になかったら場合 -> 400
	if (reqest_target.empty() || reqest_target[0] != '/') {
		throw HttpParseException(
			"Error: the request target is missing the '/' character at the beginning", BAD_REQUEST
		);
	}
}

void HttpParse::CheckValidVersion(const std::string &version) {
	// HTTP/1.1かどうか -> 400
	if (version != "HTTP/1.1") {
		throw HttpParseException("Error: The version is not supported by webserv", BAD_REQUEST);
	}
}

void HttpParse::CheckValidHeaderFieldValue(const std::string &header_field_value) {
	// C++98 では初期化リストがサポートされていないため
	static const std::string header_fields[] = {
		"Host",
		"User-Agent",
		"Accept",
		"Accept-Encoding",
		"Content-Type",
		"Content-Length",
		"Connection",
		"Authorization"
	};
	static const std::size_t header_fields_size = sizeof(header_fields) / sizeof(header_fields[0]);
	if (std::find(header_fields, header_fields + header_fields_size, header_field_value) ==
		header_fields + header_fields_size) {
		throw HttpParseException(
			"Error: the value does not exist in format of header fields", BAD_REQUEST
		);
	}
}

HttpParse::HttpParseException::HttpParseException(
	const std::string &error_message, StatusCode status_code
)
	: runtime_error(error_message), status_code_(status_code) {}

StatusCode HttpParse::HttpParseException::GetStatusCode() const {
	return status_code_;
}

// status_line && header
// messagebody

} // namespace http
