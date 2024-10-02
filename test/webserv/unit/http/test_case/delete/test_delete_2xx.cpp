#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "test_expected_response.hpp"
#include "test_handler.hpp"
#include "test_request.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <unistd.h>

namespace test {

bool HandleWhetherFileDelete(const std::string& file) {
	if (access(file.c_str(), F_OK) == 0) {
		std::cerr << utils::color::RED << "Error: cann't delete the file in delete test case." << utils::color::RESET << std::endl;
		return EXIT_FAILURE;
	} else {
		return EXIT_SUCCESS;
	}
}

int TestDeleteNoContent1ExistingFile(const server::VirtualServerAddrList &server_infos) {
	int                ret_code  = EXIT_SUCCESS;
	const std::string &file_name = "../../../../root/upload/delete_file";
	std::ofstream      file(file_name.c_str(), std::ios::binary);
	if (file.fail()) {
		utils::Debug("Error: fail to create file in delete test case.");
		return EXIT_FAILURE;
	}
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
	ret_code |= HandleHttpResult(client_infos, server_infos, expected, "204-01");
	ret_code |= HandleWhetherFileDelete(file_name);
	return ret_code;
}

int TestDeleteNoContent2ExistingFileWithBodyMessage(
	const server::VirtualServerAddrList &server_infos
) {
	int                ret_code  = EXIT_SUCCESS;
	const std::string &file_name = "../../../../root/upload/delete_file";
	std::ofstream      file(file_name.c_str(), std::ios::binary);
	if (file.fail()) {
		utils::Debug("Error: fail to create file in delete test case.");
		return EXIT_FAILURE;
	}
	http::ClientInfos client_infos =
		CreateClientInfos(request::DELETE_204_2_EXISTING_FILE_WITH_BODY_MESSAGE);
	std::string  expected_status_line  = EXPECTED_STATUS_LINE_NO_CONTENT;
	std::string  expected_body_message = EXPECTED_BODY_MESSAGE_NO_CONTENT;
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION]     = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] = utils::ToString(expected_body_message.length());
	expected_header_fields[http::CONTENT_TYPE]   = http::TEXT_HTML;
	expected_header_fields[http::SERVER]         = http::SERVER_VERSION;
	const std::string &expected_response         = CreateHttpResponseFormat(
        expected_status_line, expected_header_fields, expected_body_message
    );
	http::HttpResult expected = CreateHttpResult(true, false, "", expected_response);
	ret_code |= HandleHttpResult(client_infos, server_infos, expected, "204-02");
	ret_code |= HandleWhetherFileDelete(file_name);
	return ret_code;
}

int TestDeleteNoContent3ExistingFileThenNotFoundOnSecondAttempt(
	const server::VirtualServerAddrList &server_infos
) {
	int                ret_code  = EXIT_SUCCESS;
	const std::string &file_name = "../../../../root/upload/delete_file";
	std::ofstream      file(file_name.c_str(), std::ios::binary);
	if (file.fail()) {
		utils::Debug("Error: fail to create file in delete test case.");
		return EXIT_FAILURE;
	}
	http::ClientInfos client_infos =
		CreateClientInfos(request::DELETE_204_3_EXISTING_FILE_THEN_404_ON_SECOND_ATTEMPT);
	std::string  expected_status_line_no_content  = EXPECTED_STATUS_LINE_NO_CONTENT;
	std::string  expected_body_message_no_content = EXPECTED_BODY_MESSAGE_NO_CONTENT;
	HeaderFields expected_header_fields;
	expected_header_fields[http::CONNECTION] = http::KEEP_ALIVE;
	expected_header_fields[http::CONTENT_LENGTH] =
		utils::ToString(expected_body_message_no_content.length());
	expected_header_fields[http::CONTENT_TYPE]      = http::TEXT_HTML;
	expected_header_fields[http::SERVER]            = http::SERVER_VERSION;
	const std::string &expected_response_no_content = CreateHttpResponseFormat(
		expected_status_line_no_content, expected_header_fields, expected_body_message_no_content
	);
	std::string request_buffer =
		"DELETE /upload/delete_file HTTP/1.1\r\nHost: localhost\r\nConnection: close\r\n\r\n";
	http::HttpResult expected_no_content =
		CreateHttpResult(true, true, request_buffer, expected_response_no_content);
	ret_code |= HandleHttpResult(client_infos, server_infos, expected_no_content, "204-03-1");
	ret_code |= HandleWhetherFileDelete(file_name);

	client_infos.request_buf                    = request_buffer;
	std::string expected_status_line_not_found  = EXPECTED_STATUS_LINE_NOT_FOUND;
	std::string expected_body_message_not_found = EXPECTED_BODY_MESSAGE_NOT_FOUND;
	expected_header_fields[http::CONNECTION]    = http::CLOSE;
	expected_header_fields[http::CONTENT_LENGTH] =
		utils::ToString(expected_body_message_not_found.length());
	const std::string &expected_response_not_found = CreateHttpResponseFormat(
		expected_status_line_not_found, expected_header_fields, expected_body_message_not_found
	);
	http::HttpResult expected_not_found =
		CreateHttpResult(true, false, "", expected_response_not_found);
	ret_code |= HandleHttpResult(client_infos, server_infos, expected_not_found, "404-03-2");
	return ret_code;
}

} // namespace test
