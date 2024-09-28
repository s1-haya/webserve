#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestGetOk1ConnectionClose(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos         = CreateClientInfos(request::GET_200_1_CONNECTION_CLOSE);
	std::string       expected_status_line = EXPECTED_STATUS_LINE_OK;
	std::string       expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-01");
}

int TestGetOk2ConnectionKeep(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_200_2_CONNECTION_KEEP);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string       expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, true, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-02");
}

int TestGetOk3SubConnectionClose(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_200_3_SUB_CONNECTION_CLOSE);
	std::string       expected_status_line = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/sub/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-03");
}

int TestGetOk4ConnectionKeepAndOkConnectionKeep(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_4_CONNECTION_KEEP_AND_200_CONNECTION_KEEP);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer =
		"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n";
	http::HttpResult expected =
		CreateHttpResult(true, true, expected_request_buffer, expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-04");
}

int TestGetOk5ConnectionCloseAndOkConnectionClose(const server::VirtualServerAddrList &server_infos
) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_5_CONNECTION_CLOSE_AND_200_CONNECTION_CLOSE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer =
		"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-05");
}

int TestGetOk6ConnectionKeepAndOkConnectionClose(const server::VirtualServerAddrList &server_infos
) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_6_CONNECTION_CLOSE_AND_200_CONNECTION_CLOSE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer =
		"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult expected =
		CreateHttpResult(true, true, expected_request_buffer, expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-06");
}

int TestGetOk7DuplicateConnectionKeep(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_7_DUPLICATE_CONNECTION_KEEP);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, true, "", expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-07");
}

int TestGetOk8DuplicateConnectionClose(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_8_DUPLICATE_CONNECTION_CLOSE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-08");
}

int TestGetOk9ConnectionKeepAndClose(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_9_CONNECTION_KEEP_AND_CLOSE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-09");
}

int TestGetOk10ConnectionCloseAndKeep(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_10_CONNECTION_CLOSE_AND_KEEP);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response1        = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response1);
	return HandleHttpResult(client_infos, server_infos, expected, "200-10");
}

int TestGetOk13ExtraRequest(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_200_13_EXTRA_REQUEST);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string       expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &request_buffer = "HELLO";
	http::HttpResult   expected = CreateHttpResult(true, false, request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-13");
}

} // namespace test
