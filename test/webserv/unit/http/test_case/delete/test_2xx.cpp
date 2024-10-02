#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestDeleteNoContent1ExistingFile(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos         = CreateClientInfos(request::DELETE_204_1_EXISTING_FILE);
	std::string       expected_status_line = EXPECTED_STATUS_LINE_NO_CONTENT;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_NO_CONTENT;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "204-01");
}

int TestDeleteNoContent2ExistingFileWithBodyMessage(const server::VirtualServerAddrList &server_infos);
int TestDeleteNoContent3ExistingFileThenNotFoundOnSecondAttempt(
	const server::VirtualServerAddrList &server_infos
);

}
