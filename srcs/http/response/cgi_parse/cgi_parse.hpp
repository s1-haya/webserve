#include "cgi.hpp"
#include "http_format.hpp"
#include "result.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace http {
namespace cgi {

struct CgiRequest {
	Cgi::MetaMap meta_variables;
	std::string  body_message;
};

class CgiParse {
  public:
	static utils::Result<CgiRequest> Parse(
		const HttpRequestFormat &request,
		const std::string       &cgi_script,
		const std::string       &cgi_extension,
		const std::string       &server_port
	);

  private:
	CgiParse();
	~CgiParse();
	CgiParse(const CgiParse &other);
	CgiParse           &operator=(const CgiParse &other);
	static Cgi::MetaMap CreateRequestMetaVariables(
		const HttpRequestFormat &request,
		const std::string       &cgi_script,
		const std::string       &cgi_extension,
		const std::string       &server_port
	); // alias等を通過したパスが必要なため
};

} // namespace cgi
} // namespace http
