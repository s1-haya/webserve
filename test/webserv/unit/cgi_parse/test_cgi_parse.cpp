#include "cgi_parse.hpp"
#include "cgi_request.hpp"
#include "http_format.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

namespace {

using namespace http;

// ==================== Test汎用 ==================== //

int GetTestCaseNum() {
	static int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

template <typename T>
void IsSame(const T &a, const T &b, const char *file, int line) {
	if (a != b) {
		std::stringstream ss;
		ss << line;
		throw std::logic_error(
			std::string("Error at ") + file + ":" + ss.str() + " - Expected: " + std::string(b) +
			", Got: " + std::string(a)
		);
	}
}

/**
 * @brief Output where is the error
 */
#define COMPARE(a, b) IsSame(a, b, __FILE__, __LINE__)

template <class InputIt>
InputIt Next(InputIt it, typename std::iterator_traits<InputIt>::difference_type n = 1) {
	std::advance(it, n);
	return it;
} // std::next for c++98

// ================================================= //

// ホームから見たディレクトリパス
const std::string root_dir_path    = "root";
const std::string cgi_bin_dir_path = root_dir_path + "/cgi-bin";

// GETのテスト
int Test1() {
	// request
	const RequestLine request_line = {"GET", "/", "HTTP/1.1"};
	HttpRequestFormat request;
	request.request_line        = request_line;
	request.body_message        = "";
	request.header_fields[HOST] = "host";

	std::string cgi_script    = "root/cgi-bin/test.cgi";
	std::string cgi_path_info = "/aa/b";
	std::string cgi_extension = ".cgi";
	std::string server_port   = "8080";
	std::string client_ip     = "127.0.0.2";
	std::string server_name   = "host";

	cgi::CgiRequest cgi_request =
		CgiParse::Parse(request, cgi_script + cgi_path_info, cgi_extension, server_port, client_ip);
	cgi::MetaMap meta_variables = cgi_request.meta_variables;
	try {
		if (!request.body_message.empty()) {
			COMPARE(
				meta_variables.at(cgi::CONTENT_LENGTH), request.header_fields.at(CONTENT_LENGTH)
			);
			COMPARE(meta_variables.at(cgi::CONTENT_TYPE), request.header_fields.at(CONTENT_TYPE));
		}
		COMPARE(meta_variables.at(cgi::PATH_INFO), cgi_path_info);
		COMPARE(meta_variables.at(cgi::PATH_TRANSLATED), root_dir_path + cgi_path_info);
		COMPARE(meta_variables.at(cgi::REMOTE_ADDR), client_ip);
		COMPARE(meta_variables.at(cgi::REQUEST_METHOD), request_line.method);
		COMPARE(meta_variables.at(cgi::SCRIPT_NAME), cgi_script);
		COMPARE(meta_variables.at(cgi::SERVER_NAME), request.header_fields.at(HOST));
		COMPARE(meta_variables.at(cgi::SERVER_PORT), server_port);
		COMPARE(meta_variables.at(cgi::SERVER_PROTOCOL), request_line.version);
		COMPARE(cgi_request.body_message, request.body_message);
	} catch (const std::exception &e) {
		PrintNg();
		utils::Debug("Test1", e.what());
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug("PATH_INFO", meta_variables.at(cgi::PATH_INFO));
	utils::Debug("PATH_TRANSLATED", meta_variables.at(cgi::PATH_TRANSLATED));
	utils::Debug("SCRIPT_NAME", meta_variables.at(cgi::SCRIPT_NAME));
	return EXIT_SUCCESS;
}

// POSTのテスト
int Test2() {
	// request
	const RequestLine request_line = {"POST", "/", "HTTP/1.1"};
	HttpRequestFormat request;
	request.request_line                  = request_line;
	request.body_message                  = "test";
	request.header_fields[CONTENT_LENGTH] = "4";
	request.header_fields[CONTENT_TYPE]   = "text/plain";
	request.header_fields[HOST]           = "host";

	std::string cgi_script    = "root/cgi-bin/test2.cgi";
	std::string cgi_path_info = "";
	std::string cgi_extension = ".cgi";
	std::string server_port   = "8080";
	std::string client_ip     = "127.0.0.1";
	std::string server_name   = "host";

	cgi::CgiRequest cgi_request =
		CgiParse::Parse(request, cgi_script + cgi_path_info, cgi_extension, server_port, client_ip);
	cgi::MetaMap meta_variables = cgi_request.meta_variables;
	try {
		if (!request.body_message.empty()) {
			COMPARE(
				meta_variables.at(cgi::CONTENT_LENGTH), request.header_fields.at(CONTENT_LENGTH)
			);
			COMPARE(meta_variables.at(cgi::CONTENT_TYPE), request.header_fields.at(CONTENT_TYPE));
		}
		COMPARE(meta_variables.at(cgi::PATH_INFO), cgi_path_info);
		COMPARE(meta_variables.at(cgi::PATH_TRANSLATED), cgi_path_info);
		COMPARE(meta_variables.at(cgi::REMOTE_ADDR), client_ip);
		COMPARE(meta_variables.at(cgi::REQUEST_METHOD), request_line.method);
		COMPARE(meta_variables.at(cgi::SCRIPT_NAME), cgi_script);
		COMPARE(meta_variables.at(cgi::SERVER_NAME), request.header_fields.at(HOST));
		COMPARE(meta_variables.at(cgi::SERVER_PORT), server_port);
		COMPARE(meta_variables.at(cgi::SERVER_PROTOCOL), request_line.version);
		COMPARE(cgi_request.body_message, request.body_message);
	} catch (const std::exception &e) {
		PrintNg();
		utils::Debug("Test2", e.what());
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug("PATH_INFO", meta_variables.at(cgi::PATH_INFO));
	utils::Debug("PATH_TRANSLATED", meta_variables.at(cgi::PATH_TRANSLATED));
	utils::Debug("SCRIPT_NAME", meta_variables.at(cgi::SCRIPT_NAME));
	return EXIT_SUCCESS;
}

} // namespace

int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();
	ret |= Test2();

	return ret;
}
