#ifndef TEST_CASE_HPP_
#define TEST_CASE_HPP_

#include <list>

namespace server {

class VirtualServer;
typedef std::list<const VirtualServer *> VirtualServerAddrList;

} // namespace server

namespace test {

int TestGetOk1NoConnection(const server::VirtualServerAddrList &server_infos);
int TestGetNotFound1NotExistFile(const server::VirtualServerAddrList &server_infos);

} // namespace test

#endif
