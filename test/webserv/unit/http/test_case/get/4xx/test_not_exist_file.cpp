#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "utils.hpp"

namespace test {

int test_not_exist_file(const server::VirtualServerAddrList &server_infos) {
	const std::string &request_buffer =
		test::LoadFileContent("../../../common/request/get/404_not-exist-path_connection-close.txt"
		);
	http::ClientInfos client_infos = test::CreateClientInfos(request_buffer);
	std::string       expected1_status_line = test::EXPECTED_STATUS_LINE_NOT_FOUND;
	std::string expected1_body_message =
		test::LoadFileContent("../../expected_response/default_body_message/not_found.txt");
	std::string expected1_header_fields = test::SetDefaultHeaderFields(
		http::KEEP_ALIVE, utils::ToString(expected1_body_message.length()), "test/html"
	);
	const std::string &expected1_response =
		expected1_status_line + expected1_header_fields + http::CRLF + expected1_body_message;
	http::HttpResult expected = test::CreateHttpResult(false, true, "", "");
	return test::HandleHttpResult(client_infos, server_infos, expected);
}

} // namespace test
