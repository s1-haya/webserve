#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <stdexcept> //runtime_error
#include <string>
#include <vector>

namespace http {

struct RequestLine {
	std::string method;
	std::string request_target;
	std::string version;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpRequest {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  message_body;
};

enum StatusCode {
	OK,
	BAD_REQUEST,
	NOT_FOUND,
	NOT_IMPLEMENTED
};

struct HttpRequestResult {
	StatusCode  status_code;
	HttpRequest request;
	HttpRequestResult() : status_code(OK) {}
};

class HttpParse {
  public:
	static HttpRequestResult Run(const std::string &buf);

  private:
	HttpParse();
	~HttpParse();
	static RequestLine
	SetRequestLine(const std::vector<std::string> &request_line);
	static HeaderFields
	SetHeaderFields(const std::vector<std::string> &header_fields_info);
	static void CheckValidRequestLine(const std::vector<std::string> &request_line_info);
	static void CheckValidMethod(const std::string &method);
	static void CheckValidRequestTarget(const std::string &request_target);
	static void CheckValidVersion(const std::string &version);

	static void CheckHeaderFieldValue(const std::string &header_field_value);

	class HttpParseException : public std::runtime_error {
	  public:
		explicit HttpParseException(const std::string &error_message, StatusCode status_code);
		StatusCode GetStatusCode() const;

	  private:
		StatusCode status_code_;
	};
};

} // namespace http

#endif
