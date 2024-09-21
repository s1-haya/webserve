#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestGetNotFound1NotExistFile(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		test::CreateClientInfos(test::REQUEST_GET_404_1_NOT_EXIST_PATH_CONNECTION_CLOSE);
	std::string  expected_status_line  = test::EXPECTED_STATUS_LINE_NOT_FOUND;
	std::string  expected_body_message = test::EXPECTED_BODY_MESSAGE_NOT_FOUND;
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = "test/html";
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected1_response        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = test::CreateHttpResult(false, true, "", "");
	return test::HandleHttpResult(client_infos, server_infos, expected);
}

} // namespace test
