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

std::string StrTrimLeadingOptionalWhitespace(const std::string &str) {
	std::string::size_type pos = str.find_first_not_of(OPTIONAL_WHITESPACE);
	if (pos != std::string::npos) {
		return str.substr(pos);
	} else {
		return "";
	}
}

} // namespace

void HttpParse::ParseRequestLine(HttpRequestParsedData *data) {
	if (!data->is_request_format.is_request_line) {
		size_t pos = data->current_buf.find(CRLF);
		if (pos != std::string::npos) {
			std::string request_line = data->current_buf.substr(0, pos);
			data->request_result.request.request_line =
				SetRequestLine(utils::SplitStr(request_line, SP));
			data->is_request_format.is_request_line = true;
			data->current_buf.erase(0, pos + 2);
		}
	}
}

void HttpParse::ParseHeaderFields(HttpRequestParsedData *data) {
	if (data->is_request_format.is_request_line && !data->is_request_format.is_header_fields) {
		size_t pos = data->current_buf.find(CRLF + CRLF);
		if (pos != std::string::npos) {
			std::string header_fileds = data->current_buf.substr(0, pos);
			data->request_result.request.header_fields =
				SetHeaderFields(utils::SplitStr(header_fileds, CRLF));
			data->is_request_format.is_header_fields = true;
			data->current_buf.erase(0, pos + 4);
			if (data->request_result.request.header_fields.find("Content-Length") ==
					data->request_result.request.header_fields.end() &&
				data->request_result.request.header_fields.find("Transfer-Encoding") ==
					data->request_result.request.header_fields.end()) {
				data->is_request_format.is_body_message = true;
			}
		}
	}
}

void HttpParse::TmpRun(HttpRequestParsedData *data) {
	try {
		ParseRequestLine(data);
		ParseHeaderFields(data);
	} catch (const HttpParseException &e) {
		data->request_result.status_code = e.GetStatusCode();
	}

	// todo: is_request_lineがtrue, is_header_filedsがfalseの場合
	// if (is_request_line && !is_header_fileds) {
	// todo:
	// - current_bufをCRLFCRLFまで切り取る。
	// - HttpRequestResultを格納する。
	// - is_header_fileds = true;
	// - ContentLengthとTRANSFER_ENCORDINGがない場合 is_body_message = true;
	//}

	// todo: is_request_lineとis_header_fieldsがtrue,
	// is_bodyがfalseの場合（今回はContentLengthのみ） if (is_request_line && is_header_fileds &&
	// !is_body_message) { todo:
	// - current_bufをContentLength文まで読み取る。
	// - HttpRequestResultを格納する。
	// - is_body_message = true;
	//}
}

// todo: tmp request_
HttpRequestResult HttpParse::Run(const std::string &read_buf) {
	HttpRequestResult result;
	// a: [request_line ＋ header_fields, messagebody]
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
	CheckValidRequestLine(request_line_info);
	RequestLine request_line;
	request_line.method         = request_line_info[0];
	request_line.request_target = request_line_info[1];
	request_line.version        = request_line_info[2];
	return request_line;
}

HeaderFields HttpParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin(); it != header_fields_info.end(); ++it) {
		std::vector<std::string> header_field_name_and_value = utils::SplitStr(*it, ":");
		if (header_field_name_and_value.size() != 2) {
			throw HttpParseException(
				"Error: Missing colon or multiple colons found in header filed", BAD_REQUEST
			);
		}
		// header_field_valueを初期化してるためheader_field_nameも初期化した
		const std::string &header_field_name = header_field_name_and_value[0];
		CheckValidHeaderFieldName(header_field_name);
		const std::string &header_field_value =
			StrTrimLeadingOptionalWhitespace(header_field_name_and_value[1]);
		// to do: #189  ヘッダフィールドをパースする関数（value）-> CheckValidHeaderFieldValue
		typedef std::pair<HeaderFields::const_iterator, bool> Result;
		Result result = header_fields.insert(std::make_pair(header_field_name, header_field_value));
		if (result.second == false) {
			throw HttpParseException(
				"Error: The value already exists in header fields", BAD_REQUEST
			);
		}
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
	if (std::find(BASIC_METHODS, BASIC_METHODS + BASIC_METHODS_SIZE, method) ==
		BASIC_METHODS + BASIC_METHODS_SIZE) {
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
	if (version != HTTP_VERSION) {
		throw HttpParseException("Error: The version is not supported by webserv", BAD_REQUEST);
	}
}

void HttpParse::CheckValidHeaderFieldName(const std::string &header_field_value) {
	if (std::find(
			BASIC_HEADER_FIELDS, BASIC_HEADER_FIELDS + BASIC_HEADER_FIELDS_SIZE, header_field_value
		) == BASIC_HEADER_FIELDS + BASIC_HEADER_FIELDS_SIZE) {
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
