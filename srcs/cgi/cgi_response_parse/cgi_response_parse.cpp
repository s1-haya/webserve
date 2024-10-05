#include "cgi_response_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"

namespace cgi {

const std::string CgiResponseParse::OWS = " \t";

namespace {

bool IsVString(const std::string &str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isprint(*it)) {
			return false;
		}
	}
	return true;
}

bool HasSpace(const std::string &str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (std::isspace(*it)) {
			return true;
		}
	}
	return false;
}

utils::Result<void> CheckValidHeaderFieldNameAndValue(
	const std::string &header_field_name, const std::string &header_field_value
) {
	utils::Result<void> result(false);

	if (!header_field_name.size()) {
		utils::Debug("CgiResponseParse", "Header field name is empty");
		return result;
	}
	if (!IsVString(header_field_name) || !IsVString(header_field_value)) {
		utils::Debug(
			"CgiResponseParse", "Header field name or value have non-printable characters"
		);
		return result;
	}
	if (HasSpace(header_field_name)) {
		utils::Debug("CgiResponseParse", "Header field name has space");
		return result;
	}
	if (header_field_name == "Content-Length" &&
		!utils::ConvertStrToSize(header_field_value).IsOk()) {
		utils::Debug("CgiResponseParse", "Invalid Content-Length: " + header_field_value);
		return result;
	}
	result.Set(true);
	return result;
}

} // namespace

CgiResponseParse::CgiResponseParse() {}

CgiResponseParse::~CgiResponseParse() {}

utils::Result<CgiResponseParse::ParsedData> CgiResponseParse::Parse(const std::string &response) {
	ParsedData                parsed_data;
	utils::Result<ParsedData> result(false, parsed_data);
	size_t                    pos = response.find(http::HEADER_FIELDS_END);
	if (pos == std::string::npos) {
		utils::Debug("CgiResponseParse", "Missing header fields");
		return result;
	}
	std::string header = response.substr(0, pos + http::CRLF.size()); // CRLFも含めたいため
	utils::Result<void> parse_result = ParseHeaderFields(header, parsed_data.header_fields);
	if (!parse_result.IsOk()) {
		return result;
	}
	parse_result = ParseBody(response.substr(pos + http::HEADER_FIELDS_END.size()), parsed_data);
	if (!parse_result.IsOk()) {
		return result;
	}
	return utils::Result<ParsedData>(true, parsed_data);
}

utils::Result<void>
CgiResponseParse::ParseHeaderFields(const std::string &header, HeaderFields &header_fields) {
	utils::Result<void>    result(false);
	std::string::size_type pos = 0;
	while (pos < header.size()) {
		std::string::size_type end_of_line = header.find(http::CRLF, pos);
		if (end_of_line == std::string::npos) {
			break;
		}
		std::string line                 = header.substr(pos, end_of_line - pos);
		pos                              = end_of_line + http::CRLF.size();
		std::string::size_type colon_pos = line.find(":");
		if (colon_pos == std::string::npos) {
			utils::Debug("CgiResponseParse", "Invalid header field format: " + line);
			return result;
		}
		std::string key                         = line.substr(0, colon_pos);
		std::string value                       = line.substr(colon_pos + 1);
		value                                   = TrimOws(value);
		utils::Result<void> check_header_result = CheckValidHeaderFieldNameAndValue(key, value);
		if (!check_header_result.IsOk()) {
			return result;
		}
		std::string key   = line.substr(0, colon_pos);
		std::string value = line.substr(colon_pos + 1);
		value             = TrimOws(value);
		// todo: validation(HttpParseの処理をそのまま使う)
		header_fields[key] = value;
	}
	result.Set(true);
	return result;
}

utils::Result<void> CgiResponseParse::ParseBody(const std::string &body, ParsedData &parsed_data) {
	utils::Result<void>    result(false);
	HeaderFields::iterator it = parsed_data.header_fields.find(http::CONTENT_LENGTH);
	if (it != parsed_data.header_fields.end()) {
		// ヘッダーパースで値はチェック済み
		std::size_t content_length = utils::ConvertStrToSize(it->second).GetValue();
		if (content_length > body.size()) {
			parsed_data.body = body;
			result.Set(true);
			return result;
		}
		parsed_data.body = body.substr(0, content_length);
		result.Set(true);
		return result;
	}
	// Content-Lengthがない場合は全てのbodyを格納
	parsed_data.body = body;
	result.Set(true);
	return result;
}

std::string &CgiResponseParse::TrimOws(std::string &s) {
	// 先頭のスペースとタブを削除
	std::string::iterator it = s.begin();
	while (it != s.end() && CgiResponseParse::OWS.find(*it) != std::string::npos) {
		++it;
	}
	s.erase(s.begin(), it);

	// 末尾のスペースとタブを削除
	std::string::reverse_iterator rit = s.rbegin();
	while (rit != s.rend() && CgiResponseParse::OWS.find(*rit) != std::string::npos) {
		++rit;
	}
	s.erase(rit.base(), s.end());

	return s;
}

} // namespace cgi
