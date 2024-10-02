#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

// 4xx
int TestDelete1ForbiddenDirectory(const server::VirtualServerAddrList &server_infos);
int TestDelete1NotFoundNonexistentFile(const server::VirtualServerAddrList &server_infos);
int TestDeleteMethodNotAllowed(const server::VirtualServerAddrList &server_infos);

} // namespace test
