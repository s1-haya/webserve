#ifndef TEST_HANDLER_HPP_
#define TEST_HANDLER_HPP_

#include <list>

namespace server {

class VirtualServer;
typedef std::list<const VirtualServer *> VirtualServerAddrList;

} // namespace server

namespace http {

struct ClientInfos;
struct HttpResult;

} // namespace http

namespace test {

std::string LoadFileContent(const char *file_path);
// tmp
std::string SetDefaultHeaderFields(
	const std::string &connection, const std::string &length, const std::string &type
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
	const http::HttpResult               expected
);

} // namespace test

#endif
