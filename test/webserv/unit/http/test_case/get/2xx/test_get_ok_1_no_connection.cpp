#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestGetOk1NoConnection(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = test::CreateClientInfos(test::REQUEST_GET_200_1_NO_CONNECTION);
	std::string       expected_status_line = test::EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = test::LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = "test/html";
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = test::CreateHttpResult(false, true, "", "");
	return test::HandleHttpResult(client_infos, server_infos, expected);
}

} // namespace test
