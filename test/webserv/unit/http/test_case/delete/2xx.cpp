#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestDeleteOk1ExistingFile(const server::VirtualServerAddrList &server_infos);
int TestDeleteOk2ExistingFileWithBodyMessage(const server::VirtualServerAddrList &server_infos);
int TestDeleteOk3ExistingFileThenNotFoundOnSecondAttempt(
	const server::VirtualServerAddrList &server_infos
);

}
