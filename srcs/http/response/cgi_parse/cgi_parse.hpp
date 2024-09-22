#ifndef CGI_PARSE_HPP_
#define CGI_PARSE_HPP_

#include "cgi_request.hpp"
#include "result.hpp"
#include <map>
#include <string>

namespace http {

struct HttpRequestFormat;

class CgiParse {
  public:
	static utils::Result<cgi::CgiRequest> Parse(
		const http::HttpRequestFormat &request,
		const std::string             &cgi_script,
		const std::string             &cgi_extension,
		const std::string             &server_port
	);

  private:
	CgiParse();
	~CgiParse();
	CgiParse(const CgiParse &other);
	CgiParse           &operator=(const CgiParse &other);
	static cgi::MetaMap CreateRequestMetaVariables(
		const http::HttpRequestFormat &request,
		const std::string             &cgi_script,
		const std::string             &cgi_extension,
		const std::string             &server_port
	); // alias等を通過したパスが必要なため、requestのpathではなくcgi_scriptをpathとして使用する
};

} // namespace http

#endif
