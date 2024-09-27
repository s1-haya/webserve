#include "test_handler.hpp"
#include "client_infos.hpp"
#include "http.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include <cstdlib>
#include <fstream>

namespace test {

#define DEFAULT_FD 1

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

std::string LoadFileContent(const std::string &file_path) {
	std::ifstream file(file_path.c_str(), std::ios::binary);
	if (!file) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		return "";
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	return file_content.str();
}

std::string CreateHttpResponseFormat(
	const std::string  &status_line,
	const HeaderFields &header_fields,
	const std::string  &body_message
) {
	std::string response;
	response += status_line;
	typedef HeaderFields::const_iterator Itr;
	for (Itr it = header_fields.begin(); it != header_fields.end(); ++it) {
		response += it->first + ": " + it->second + http::CRLF;
	}
	response += http::CRLF;
	response += body_message;
	return response;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

int HandleResult(const Result &result, const std::string& current_number) {
	if (result.is_success) {
		std::cout << utils::color::GREEN << current_number << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << current_number << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << result.error_log;
		return EXIT_FAILURE;
	}
}

http::ClientInfos CreateClientInfos(const std::string &request_buffer) {
	http::ClientInfos client_infos;
	client_infos.fd          = DEFAULT_FD;
	client_infos.request_buf = request_buffer;
	return client_infos;
}

http::HttpResult CreateHttpResult(
	bool               is_response_complete,
	bool               is_connection_keep,
	const std::string &request_buffer,
	const std::string &response
) {
	http::HttpResult result;
	result.is_response_complete = is_response_complete;
	result.is_connection_keep   = is_connection_keep;
	result.request_buf          = request_buffer;
	result.response             = response;
	return result;
}

Result IsSameIsResponseComplete(bool is_response_complete, bool expected) {
	Result is_response_complete_result;
	if (is_response_complete != expected) {
		std::ostringstream error_log;
		error_log << "Error: Is Response Complete\n";
		error_log << "- Expected: [" << std::boolalpha << expected << "]\n";
		error_log << "- Result  : [" << std::boolalpha << is_response_complete << "]\n";
		is_response_complete_result.is_success = false;
		is_response_complete_result.error_log  = error_log.str();
	}
	return is_response_complete_result;
}

Result IsSameIsConnectionKeep(bool is_connection_keep, bool expected) {
	Result is_connection_keep_result;
	if (is_connection_keep != expected) {
		std::ostringstream error_log;
		error_log << "Error: Is Connection Keep\n";
		error_log << "- Expected: [" << std::boolalpha << expected << "]\n";
		error_log << "- Result  : [" << std::boolalpha << is_connection_keep << "]\n";
		is_connection_keep_result.is_success = false;
		is_connection_keep_result.error_log  = error_log.str();
	}
	return is_connection_keep_result;
}

Result IsSameRequestBuffer(const std::string &request_buffer, const std::string &expected) {
	Result request_buffer_result;
	if (request_buffer != expected) {
		std::ostringstream error_log;
		error_log << "Error: Request Buffer\n";
		error_log << "- Expected: [\n" << expected << "]\n";
		error_log << "- Result  : [\n" << request_buffer << "]\n";
		request_buffer_result.is_success = false;
		request_buffer_result.error_log  = error_log.str();
	}
	return request_buffer_result;
}

Result IsSameHttpResponse(const std::string &response, const std::string &expected) {
	Result http_response_result;
	if (response != expected) {
		std::ostringstream error_log;
		error_log << "Error: Http Response\n";
		error_log << "- Expected: [\n" << expected << "]\n";
		error_log << "- Result  : [\n" << response << "]\n";
		http_response_result.is_success = false;
		http_response_result.error_log  = error_log.str();
	}
	return http_response_result;
}

Result IsSameHttpResult(const http::HttpResult &http_result, const http::HttpResult expected) {
	Result result;
	result =
		IsSameIsResponseComplete(http_result.is_response_complete, expected.is_response_complete);
	if (!result.is_success) {
		return result;
	}
	result = IsSameIsConnectionKeep(http_result.is_connection_keep, expected.is_connection_keep);
	if (!result.is_success) {
		return result;
	}
	result = IsSameRequestBuffer(http_result.request_buf, expected.request_buf);
	if (!result.is_success) {
		return result;
	}
	result = IsSameHttpResponse(http_result.response, expected.response);
	return result;
}

int HandleHttpResult(
	const http::ClientInfos             &client_infos,
	const server::VirtualServerAddrList &server_infos,
	const http::HttpResult               expected,
	const std::string                   &current_number
) {
	http::Http       http;
	http::HttpResult http_result = http.Run(client_infos, server_infos);
	const Result    &result      = IsSameHttpResult(http_result, expected);
	return HandleResult(result, current_number);
}

} // namespace test
