#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestGetBadRequest1OnlyCrlf(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_1_ONLY_CRLF);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-01");
}

int TestGetBadRequest2LowerMethod(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_2_LOWER_METHOD);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-02");
}

int TestGetBadRequest3NoAsciiMethod(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_3_NO_ASCII_METHOD);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-03");
}

int TestGetBadRequest4NoRoot(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_4_NO_ROOT);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-04");
}

int TestGetBadRequest5RelativePath(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_5_RELATIVE_PATH);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-05");
}

int TestGetBadRequest6LowerHttpVersion(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_6_LOWER_HTTP_VERSION);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-06");
}

int TestGetBadRequest7WrongHttpName(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_7_WRONG_HTTP_NAME);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-07");
}

int TestGetBadRequest8WrongHttpVersion(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_8_WRONG_HTTP_VERSION);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer = "Host: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult   expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-08");
}

int TestGetBadRequest9NoHost(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_9_NO_HOST);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-09");
}

int TestGetBadRequest10DuplicateHost(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_400_10_DUPLICATE_HOST);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-10");
}

int TestGetBadRequest11NoHeaderFieldColon(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_11_NO_HEADER_FIELD_COLON);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-11");
}

int TestGetBadRequest12NoConnectionName(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_12_NO_CONNECTION_NAME);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-12");
}

int TestGetBadRequest13NoConnectionValue(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_13_NO_CONNECTION_VALUE);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-13");
}

int TestGetBadRequest14WrongConnectionValue(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_14_WRONG_CONNECTION_VALUE);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-14");
}

int TestGetBadRequest15NotExistHeaderField(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_15_NOT_EXIST_HEADER_FIELD);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-15");
}

int TestGetBadRequest16HeaderFieldNameSpaceColon(const server::VirtualServerAddrList &server_infos
) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_400_16_HEADER_FIELD_NAME_SPACE_COLON);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string  expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-16");
}

int TestGetBadRequest17SpaceHeaderFieldName(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_400_17_SPACE_HEADER_FIELD_NAME);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_BAD_REQUEST;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_BAD_REQUEST;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "400-17");
}

int TestGetNotFound1NotExistFile(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_404_1_NOT_EXIST_PATH_CONNECTION_CLOSE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_NOT_FOUND;
	std::string  expected_body_message = EXPECTED_BODY_MESSAGE_NOT_FOUND;
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "404-01");
}

int TestGetMethodNotAllowed(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_405_1_NOT_ALLOWED);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_METHOD_NOT_ALLOWED;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_METHOD_NOT_ALLOWED;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "405-01");
}

int TestGetTimeout1NoCrlf(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_408_1_NO_CRLF);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_TIMEOUT;
	std::string       expected_body_message = EXPECTED_BODY_MESSAGE_TIMEOUT;
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, true, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "408-01");
}

} // namespace test
