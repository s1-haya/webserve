#include "result.hpp"
#include <map>
#include <string>

namespace http {

struct HttpRequestFormat;

} // namespace http

namespace cgi {

typedef std::map<std::string, std::string> MetaMap;

// namespace cgi用
extern const std::string AUTH_TYPE;
extern const std::string CONTENT_LENGTH;
extern const std::string CONTENT_TYPE;
extern const std::string GATEWAY_INTERFACE;
extern const std::string PATH_INFO;
extern const std::string PATH_TRANSLATED;
extern const std::string QUERY_STRING;
extern const std::string REMOTE_ADDR;
extern const std::string REMOTE_HOST;
extern const std::string REMOTE_IDENT;
extern const std::string REMOTE_USER;
extern const std::string REQUEST_METHOD;
extern const std::string SCRIPT_NAME;
extern const std::string SERVER_NAME;
extern const std::string SERVER_PORT;
extern const std::string SERVER_PROTOCOL;
extern const std::string SERVER_SOFTWARE;

struct CgiRequest {
	MetaMap     meta_variables;
	std::string body_message;
};

class CgiParse {
  public:
	static utils::Result<CgiRequest> Parse(
		const http::HttpRequestFormat &request,
		const std::string       &cgi_script,
		const std::string       &cgi_extension,
		const std::string       &server_port
	);

  private:
	CgiParse();
	~CgiParse();
	CgiParse(const CgiParse &other);
	CgiParse      &operator=(const CgiParse &other);
	static MetaMap CreateRequestMetaVariables(
		const http::HttpRequestFormat &request,
		const std::string       &cgi_script,
		const std::string       &cgi_extension,
		const std::string       &server_port
	); // alias等を通過したパスが必要なため、requestのpathではなくcgi_scriptをpathとして使用する
};

} // namespace cgi
