#ifndef CGI_RESPONSE_PARSE_HPP_
#define CGI_RESPONSE_PARSE_HPP_

#include "result.hpp"
#include <map>
#include <string>

namespace cgi {

class CgiResponseParse {
  public:
	typedef std::map<std::string, std::string> HeaderFields;
	struct ParsedData {
		HeaderFields header_fields;
		std::string  body;
	};

	static utils::Result<ParsedData> Parse(const std::string &response);

  private:
	CgiResponseParse();
	~CgiResponseParse();

	static utils::Result<void>
	ParseHeaderFields(const std::string &header, HeaderFields &header_fields);
	static utils::Result<void> ParseBody(const std::string &body, ParsedData &parsed_data);
};

} // namespace cgi

#endif
