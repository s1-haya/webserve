#ifndef CGI_REQUEST_HPP_
#define CGI_REQUEST_HPP_

#include <map>
#include <string>

namespace cgi {

typedef std::map<std::string, std::string> MetaMap;
struct CgiRequest {
	MetaMap     meta_variables;
	std::string body_message;
};

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

} // namespace cgi

#endif
