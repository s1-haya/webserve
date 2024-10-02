#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

// 4xx
int TestDelete1ForbiddenDirectory(const server::VirtualServerAddrList &server_infos) {
	int                ret_code  = EXIT_SUCCESS;
	http::ClientInfos client_infos =
		CreateClientInfos(request::DELETE_403_1_DIRECTORY);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_FORBIDDEN;
	std::string  expected_body_message = EXPECTED_BODY_MESSAGE_FORBIDDEN;
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	ret_code |= HandleHttpResult(client_infos, server_infos, expected, "403-01");
	return ret_code;
}

int TestDelete1NotFoundNonexistentFile(const server::VirtualServerAddrList &server_infos);
int TestDeleteMethodNotAllowed(const server::VirtualServerAddrList &server_infos);

} // namespace test
