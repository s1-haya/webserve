#include "cgi_response_parse.hpp"
#include "utils.hpp"

namespace cgi {

const std::string CgiResponseParse::CRLF              = "\r\n";
const std::string CgiResponseParse::HEADER_FIELDS_END = CRLF + CRLF;
const std::string CgiResponseParse::OWS               = " \t";

CgiResponseParse::CgiResponseParse() {}

CgiResponseParse::~CgiResponseParse() {}

utils::Result<CgiResponseParse::ParsedData> CgiResponseParse::Parse(const std::string &response) {
	ParsedData                parsed_data;
	utils::Result<ParsedData> result(false, parsed_data);
	if (response.empty()) {
		utils::Debug("CgiResponseParse", "Empty response");
		return result;
	}
	size_t pos = response.find(HEADER_FIELDS_END);
	if (pos == std::string::npos) {
		return result;
	}
	std::string         header = response.substr(0, pos + CRLF.size()); // CRLFも含めたいため
	utils::Result<void> parse_result = ParseHeaderFields(header, parsed_data.header_fields);
	if (!parse_result.IsOk()) {
		return result;
	}
	parse_result = ParseBody(response.substr(pos + HEADER_FIELDS_END.size()), parsed_data);
	if (!parse_result.IsOk()) {
		return result;
	}
	return utils::Result<ParsedData>(true, parsed_data);
}

utils::Result<void>
CgiResponseParse::ParseHeaderFields(const std::string &header, HeaderFields &header_fields) {
	utils::Result<void>    result(false);
	std::string::size_type pos = 0;
	// todo: err処理
	while (pos < header.size()) {
		std::string::size_type end_of_line = header.find(CRLF, pos);
		if (end_of_line == std::string::npos) {
			break;
		}
		std::string line                 = header.substr(pos, end_of_line - pos);
		pos                              = end_of_line + CRLF.size();
		std::string::size_type colon_pos = line.find(":");
		if (colon_pos == std::string::npos) {
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
	HeaderFields::iterator it = parsed_data.header_fields.find("Content-Length");
	if (it != parsed_data.header_fields.end()) {
		utils::Result<std::size_t> convert_result = utils::ConvertStrToSize(it->second);
		if (!convert_result.IsOk()) {
			return result;
		}
		std::size_t content_length = convert_result.GetValue();
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
