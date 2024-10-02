#include "cgi_response_parse.hpp"
#include "utils.hpp"

namespace http {

const std::string CgiResponseParse::CRLF              = "\r\n";
const std::string CgiResponseParse::HEADER_FIELDS_END = CRLF + CRLF;
const std::string CgiResponseParse::OWS               = " \t";

CgiResponseParse::CgiResponseParse() {}

CgiResponseParse::~CgiResponseParse() {}

CgiResponseParse::ParsedData CgiResponseParse::Parse(const std::string &response) {
	parsed_data_.header_fields.clear();
	parsed_data_.body.clear();
	size_t pos = response.find(HEADER_FIELDS_END);
	if (pos == std::string::npos) {
		return parsed_data_;
	}
	std::string header = response.substr(0, pos + CRLF.size()); // CRLFも含めたいため
	ParseHeaderFields(header);
	ParseBody(response.substr(pos + HEADER_FIELDS_END.size()));
	return parsed_data_;
}

void CgiResponseParse::ParseHeaderFields(const std::string &header) {
	std::string::size_type pos = 0;
	// todo: err でthrow
	while (pos < header.size()) {
		std::string::size_type end_of_line = header.find(CRLF, pos);
		if (end_of_line == std::string::npos) {
			break;
		}
		std::string line                 = header.substr(pos, end_of_line - pos);
		pos                              = end_of_line + CRLF.size();
		std::string::size_type colon_pos = line.find(":");
		if (colon_pos == std::string::npos) {
			continue;
		}
		std::string key   = line.substr(0, colon_pos);
		std::string value = line.substr(colon_pos + 1);
		value             = TrimOWS(value);
		// todo: validation(HttpParseの処理をそのまま使う)
		parsed_data_.header_fields[key] = value;
	}
}

void CgiResponseParse::ParseBody(const std::string &body) {
	HeaderFields::iterator it = parsed_data_.header_fields.find("Content-Length");
	if (it != parsed_data_.header_fields.end()) {
		std::string::size_type content_length = utils::ConvertStrToSize(it->second).GetValue();
		// todo: ConvertStrToSize is Not Ok
		if (content_length > body.size()) {
			parsed_data_.body = body;
			return;
		}
		parsed_data_.body = body.substr(0, content_length);
		return;
	}
}

std::string &CgiResponseParse::TrimOWS(std::string &s) {
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

} // namespace http
