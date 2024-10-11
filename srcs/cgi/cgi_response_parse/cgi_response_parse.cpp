#include "cgi_response_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"

namespace cgi {
namespace {

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
	if (!utils::IsVString(header_field_name) || !utils::IsVString(header_field_value)) {
		utils::Debug(
			"CgiResponseParse", "Header field name or value have non-printable characters"
		);
		return result;
	}
	if (HasSpace(header_field_name)) {
		utils::Debug("CgiResponseParse", "Header field name has space");
		return result;
	}
	if (header_field_name == http::CONTENT_LENGTH &&
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
	if (response.empty()) {
		utils::Debug("CgiResponseParse", "Empty response");
		return result;
	}
	size_t pos = response.find(http::HEADER_FIELDS_END);
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
		std::string key   = utils::ToLowerString(line.substr(0, colon_pos));
		std::string value = utils::Trim(
			utils::ToLowerString(line.substr(colon_pos + 1)), http::OPTIONAL_WHITESPACE
		);
		utils::Result<void> check_header_result = CheckValidHeaderFieldNameAndValue(key, value);
		if (!check_header_result.IsOk()) {
			return result;
		}
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

} // namespace cgi
