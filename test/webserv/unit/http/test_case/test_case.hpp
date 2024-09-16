#ifndef TEST_CASE_HPP_
#define TEST_CASE_HPP_

#include <list>

namespace server {

class VirtualServer;
typedef std::list<const VirtualServer *> VirtualServerAddrList;

} // namespace server

namespace test {

int test_no_connection(const server::VirtualServerAddrList &server_infos);

}

#endif
