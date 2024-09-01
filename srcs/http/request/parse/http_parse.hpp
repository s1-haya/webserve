#ifndef HTTP_PARSE_HPP_
#define HTTP_PARSE_HPP_

#include "http_exception.hpp"
#include "http_format.hpp"
#include <map>
#include <stdexcept> //runtime_error
#include <string>
#include <vector>

namespace http {

struct HttpRequestResult {
	EStatusCode       status_code;
	HttpRequestFormat request;
	HttpRequestResult() : status_code(OK) {}
};

struct IsHttpRequestFormat {
	IsHttpRequestFormat()
		: is_request_line(false), is_header_fields(false), is_body_message(false){};
	bool is_request_line;
	bool is_header_fields;
	bool is_body_message;
};

struct HttpRequestParsedData {
	// HTTP各書式のパースしたかどうか
	IsHttpRequestFormat is_request_format;
	// HttpRequestResult
	HttpRequestResult request_result;
	// HTTP各書式をパースする前の読み込んだ情報
	std::string current_buf;
};

class HttpParse {
  public:
	static HttpRequestResult Run(const std::string &read_buf);
	static void              TmpRun(HttpRequestParsedData &data);
	static void              TmpRunHttpResultVersion(HttpRequestParsedData &data);

  private:
	HttpParse();
	~HttpParse();
	static void ParseRequestLine(HttpRequestParsedData &data);
	static void ParseHeaderFields(HttpRequestParsedData &data);
	static void ParseBodyMessage(HttpRequestParsedData &data);

	static RequestLine  SetRequestLine(const std::vector<std::string> &request_line);
	static HeaderFields SetHeaderFields(const std::vector<std::string> &header_fields_info);
	static void         CheckValidRequestLine(const std::vector<std::string> &request_line_info);
	static void         CheckValidMethod(const std::string &method);
	static void         CheckValidRequestTarget(const std::string &request_target);
	static void         CheckValidVersion(const std::string &version);

	static void CheckValidHeaderFieldName(const std::string &header_field_value);
};

} // namespace http

#endif
