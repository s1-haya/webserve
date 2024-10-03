#ifndef CGI_RESPONSE_PARSE_HPP_
#define CGI_RESPONSE_PARSE_HPP_

#include <map>
#include <string>

namespace http {

class CgiResponseParse {
  public:
	typedef std::map<std::string, std::string> HeaderFields;
	struct ParsedData {
		HeaderFields header_fields;
		std::string  body;
	};

	static ParsedData Parse(const std::string &response);

  private:
	CgiResponseParse();
	~CgiResponseParse();

	static void         ParseHeaderFields(const std::string &header, ParsedData &parsed_data);
	static void         ParseBody(const std::string &body, ParsedData &parsed_data);
	static std::string &TrimOws(std::string &s);

	static const std::string CRLF;
	static const std::string HEADER_FIELDS_END;
	static const std::string OWS;
};

} // namespace http

#endif
