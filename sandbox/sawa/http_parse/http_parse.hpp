#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include <map>
#include <string>
#include <vector>

namespace http {

struct RequestLine {
	std::string method;
	std::string request_target;
	std::string version;
	// Fot test(main.cpp)
	RequestLine() : method(""), request_target(""), version("") {}
	RequestLine(
		const std::string &method, const std::string &request_target, const std::string &version
	)
		: method(method), request_target(request_target), version(version) {}
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpRequest {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  body_message;
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
	SetRequestLine(const std::vector<std::string> &request_line, StatusCode *status_code);
	static HeaderFields
	SetHeaderFields(const std::vector<std::string> &header_fields_info, StatusCode *status_code);

	static std::string CheckMethod(const std::string &method);
	static std::string CheckRequestTarget(const std::string &request_target);
	static std::string CheckVersion(const std::string &version);

	static std::string CheckHeaderFieldValue(const std::string &header_field_value);
	class HttpParseException {
	  public:
		explicit HttpParseException(StatusCode status_code);
		StatusCode GetStatusCode() const;

	  private:
		StatusCode status_code_;
	};
};

} // namespace http

#endif
