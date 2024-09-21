#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestGetOk1NoConnection(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = test::CreateClientInfos(test::REQUEST_GET_200_NO_CONNECTION);
	std::string       expected1_status_line = test::EXPECTED_STATUS_LINE_OK;
	std::string expected1_body_message  = test::LoadFileContent("../../../../root/html/index.html");
	std::string expected1_header_fields = test::SetDefaultHeaderFields(
		http::KEEP_ALIVE, utils::ToString(expected1_body_message.length()), "test/html"
	);
	const std::string &expected1_response =
		expected1_status_line + expected1_header_fields + http::CRLF + expected1_body_message;
	http::HttpResult expected = test::CreateHttpResult(false, true, "", "");
	return test::HandleHttpResult(client_infos, server_infos, expected);
}

} // namespace test
