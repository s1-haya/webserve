#ifndef TEST_CASE_HPP_
#define TEST_CASE_HPP_

#include <list>

namespace server {

class VirtualServer;
typedef std::list<const VirtualServer *> VirtualServerAddrList;

} // namespace server

namespace test {

// 2xx
int TestGetOk1ConnectionClose(const server::VirtualServerAddrList &server_infos);
// 4xx
int TestGetBadRequest1OnlyCrlf(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest2LowerMethod(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest3NoAsciiMethod(const server::VirtualServerAddrList &server_infos);
int TestGetBadRequest4NoRoot(const server::VirtualServerAddrList &server_infos);
int TestGetNotFound1NotExistFile(const server::VirtualServerAddrList &server_infos);
int TestGetMethodNotAllowed(const server::VirtualServerAddrList &server_infos);
int TestGetTimeout1NoCrlf(const server::VirtualServerAddrList &server_infos);
// 5xx
int TestGetNotImplemented1NotExistMethod(const server::VirtualServerAddrList &server_infos);

} // namespace test

#endif
