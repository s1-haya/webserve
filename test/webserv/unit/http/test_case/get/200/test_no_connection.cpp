#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_handler.hpp"
#include "utils.hpp"

namespace test {

int test_no_connection(const server::VirtualServerAddrList &server_infos) {
	const std::string &request_buffer =
		test::LoadFileContent("../../../common/request/get/200/no-connection.txt");
	http::ClientInfos client_infos = test::CreateClientInfos(request_buffer);
	std::string       expected1_status_line =
		test::LoadFileContent("../../expected_response/default_status_line/ok.txt");
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
