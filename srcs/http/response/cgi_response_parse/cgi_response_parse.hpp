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

	CgiResponseParse();
	~CgiResponseParse();
	ParsedData Parse(const std::string &response);

  private:
	CgiResponseParse(const CgiResponseParse &other);
	CgiResponseParse &operator=(const CgiResponseParse &other);

	void         ParseHeaderFields(const std::string &header);
	void         ParseBody(const std::string &body);
	std::string &TrimOWS(std::string &s);

	ParsedData parsed_data_;

	static const std::string CRLF;
	static const std::string HEADER_FIELDS_END;
	static const std::string OWS;
};

} // namespace http

#endif
