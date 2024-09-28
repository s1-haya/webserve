#ifndef TEST_HANDLER_HPP_
#define TEST_HANDLER_HPP_

#include <list>
#include <map>
#include <string>

namespace server {

class VirtualServer;
typedef std::list<const VirtualServer *> VirtualServerAddrList;

} // namespace server

namespace http {

struct ClientInfos;
struct HttpResult;

} // namespace http

namespace test {

typedef std::map<std::string, std::string> HeaderFields;

std::string LoadFileContent(const std::string &file_path);
std::string CreateHttpResponseFormat(
	const std::string  &status_line,
	const HeaderFields &header_fields,
	const std::string  &body_message
);
http::ClientInfos CreateClientInfos(const std::string &request_buffer);
http::HttpResult  CreateHttpResult(
	 bool               is_response_complete,
	 bool               is_connection_keep,
	 const std::string &request_buffer,
	 const std::string &response
 );
int HandleHttpResult(
	const http::ClientInfos             &client_infos,
	const server::VirtualServerAddrList &server_infos,
	const http::HttpResult               expected,
	const std::string                   &current_number
);

} // namespace test

#endif
