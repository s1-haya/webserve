#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"

namespace test {

int TestCgiGetOk1PrintOkBodyMessage(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos =
		CreateClientInfos(request::cgi::GET_200_01_PRINT_OK_PL_BODY_MESSAGE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_OK;
	std::string  expected_body_message = "OK";
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_PLAIN;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	return HandleHttpResult(client_infos, server_infos, expected, "200-01");
}

int TestCgiGetOk2PrintOkInPerl(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::cgi::GET_200_02_PRINT_OK_PERL);
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
	return HandleHttpResult(client_infos, server_infos, expected, "200-02");
}

int TestCgiGetOk3PrintOkInPython(const server::VirtualServerAddrList &server_infos) {
	http::ClientInfos client_infos = CreateClientInfos(request::cgi::GET_200_03_PRINT_OK_PYTHON);
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
	return HandleHttpResult(client_infos, server_infos, expected, "200-03");
}

} // namespace test
