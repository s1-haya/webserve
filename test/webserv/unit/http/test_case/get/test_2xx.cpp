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
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer =
		"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: keep-alive\r\n\r\n";
	http::HttpResult expected =
		CreateHttpResult(true, true, expected_request_buffer, expected_response);
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
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer =
		"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult expected =
		CreateHttpResult(true, false, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-05");
}

int TestGetOk6ConnectionKeepAndOkConnectionClose(const server::VirtualServerAddrList &server_infos
) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_6_CONNECTION_KEEP_AND_200_CONNECTION_CLOSE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	const std::string &expected_request_buffer =
		"GET / HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult expected =
		CreateHttpResult(true, true, expected_request_buffer, expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-06");
}

int TestGetOk11UpperAndLowerHeaderFields(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_11_UPPER_AND_LOWER_HEADER_FIELDS);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-11");
}

int TestGetOk12HeaderFieldValueSpace(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_12_HEADER_FIELD_VALUE_SPACE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-12");
}

int TestGetOk13SpaceHeaderFieldValue(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::GET_200_13_SPACE_HEADER_FIELD_VALUE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = LoadFileContent("../../../../root/html/index.html");
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-13");
}

int TestGetOk14ExtraRequest(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_200_14_EXTRA_REQUEST);
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
	return HandleHttpResult(client_infos, server_infos, expected, "200-14");
}

int TestGetOk15BodyMessageDefault(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_200_15_BODY_MESSAGE_DEFAULT);
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
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-15");
}

int TestGetOk17NotExistHeaderField(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_200_17_NOT_EXIST_HEADER_FIELD);
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
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-17");
}

int TestGetOk19CgiScriptInPython(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_200_19_CGI_SCRIPT_IN_PYTHON);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string       expected_body_message = "OK";
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_PLAIN;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-19");
}

int TestGetOk20CgiScriptInShell(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::GET_200_20_CGI_SCRIPT_IN_SHELL);
	std::string       expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string       expected_body_message = "OK";
	HeaderFields      expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_PLAIN;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-20");
}

int TestGetOk21NoConnection(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos          = CreateClientInfos(request::GET_200_21_NO_CONNECTION);
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
	return HandleHttpResult(client_infos, server_infos, expected, "200-21");
}

} // namespace test
