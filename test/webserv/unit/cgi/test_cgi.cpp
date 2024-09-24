#include "cgi.hpp"
#include "cgi_parse.hpp"
#include "cgi_request.hpp"
#include "http_format.hpp"
#include "http_message.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <unistd.h>

namespace {

using namespace cgi;

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

// ================================================= //

// cgi_parseから見た相対パス
const std::string html_dir_path    = "../../../../root/html";
const std::string cgi_bin_dir_path = "../../../../root/cgi-bin";
const int         BUF_SIZE         = 1024;

typedef std::map<int, int> PFdMap;

// 出力は目で見て確認(実行が成功していたらテストはOKとしている)

/* exec /cgi-bin/env.py */
int Test1() {
	// request
	CgiRequest cgi_request;
	cgi_request.meta_variables[CONTENT_LENGTH] = "";
	cgi_request.meta_variables[CONTENT_TYPE]   = "";
	cgi_request.meta_variables[PATH_INFO]      = "/aa/b";
	cgi_request.meta_variables[PATH_TRANSLATED] =
		html_dir_path + cgi_request.meta_variables.at(PATH_INFO);
	cgi_request.meta_variables[REQUEST_METHOD]  = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]     = cgi_bin_dir_path + "/env.py";
	cgi_request.meta_variables[SERVER_NAME]     = "localhost";
	cgi_request.meta_variables[SERVER_PORT]     = "8080";
	cgi_request.meta_variables[SERVER_PROTOCOL] = "HTTP/1.1";

	CgiResponse response;
	try {
		Cgi    cgi(cgi_request);
		PFdMap pfd_map = cgi.Run();
		if (cgi.IsWriteRequired()) {
			write(
				pfd_map.at(cgi::Cgi::WRITE),
				cgi_request.body_message.c_str(),
				cgi_request.body_message.length()
			);
		}
		while (!response.is_response_complete) {
			char    buffer[BUF_SIZE] = {};
			ssize_t read_bytes       = read(pfd_map.at(cgi::Cgi::READ), buffer, BUF_SIZE);
			response                 = cgi.AddAndGetResponse(std::string(buffer, read_bytes));
		}
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug(response.response);
	return EXIT_SUCCESS;
}

/* exec /cgi-bin/test.sh */
int Test2() {
	// request
	CgiRequest cgi_request;
	cgi_request.meta_variables[CONTENT_LENGTH] = "";
	cgi_request.meta_variables[CONTENT_TYPE]   = "";
	cgi_request.meta_variables[PATH_INFO]      = "/aa/b";
	cgi_request.meta_variables[PATH_TRANSLATED] =
		html_dir_path + cgi_request.meta_variables.at(PATH_INFO);
	cgi_request.meta_variables[REQUEST_METHOD]  = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]     = cgi_bin_dir_path + "/test.sh";
	cgi_request.meta_variables[SERVER_NAME]     = "localhost";
	cgi_request.meta_variables[SERVER_PORT]     = "8080";
	cgi_request.meta_variables[SERVER_PROTOCOL] = "HTTP/1.1";

	CgiResponse response;
	try {
		Cgi    cgi(cgi_request);
		PFdMap pfd_map = cgi.Run();
		if (cgi.IsWriteRequired()) {
			write(
				pfd_map.at(cgi::Cgi::WRITE),
				cgi_request.body_message.c_str(),
				cgi_request.body_message.length()
			);
		}
		while (!response.is_response_complete) {
			char    buffer[BUF_SIZE] = {};
			ssize_t read_bytes       = read(pfd_map.at(cgi::Cgi::READ), buffer, BUF_SIZE);
			response                 = cgi.AddAndGetResponse(std::string(buffer, read_bytes));
		}
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug(response.response);
	return EXIT_SUCCESS;
}

/* exec /cgi-bin/print_stdin.pl */
/* Test for POST method */
int Test3() {
	// request
	CgiRequest cgi_request;
	cgi_request.body_message                   = "test test";
	cgi_request.meta_variables[CONTENT_LENGTH] = utils::ToString(cgi_request.body_message.length());
	cgi_request.meta_variables[CONTENT_TYPE]   = "text/plain";
	cgi_request.meta_variables[PATH_INFO]      = "/aa/b";
	cgi_request.meta_variables[PATH_TRANSLATED] =
		html_dir_path + cgi_request.meta_variables.at(PATH_INFO);
	cgi_request.meta_variables[REQUEST_METHOD]  = "POST";
	cgi_request.meta_variables[SCRIPT_NAME]     = cgi_bin_dir_path + "/print_stdin.pl";
	cgi_request.meta_variables[SERVER_NAME]     = "localhost";
	cgi_request.meta_variables[SERVER_PORT]     = "8080";
	cgi_request.meta_variables[SERVER_PROTOCOL] = "HTTP/1.1";

	CgiResponse response;
	try {
		Cgi    cgi(cgi_request);
		PFdMap pfd_map = cgi.Run();
		if (cgi.IsWriteRequired()) {
			write(
				pfd_map.at(cgi::Cgi::WRITE),
				cgi_request.body_message.c_str(),
				cgi_request.body_message.length()
			);
		}
		while (!response.is_response_complete) {
			char    buffer[BUF_SIZE] = {};
			ssize_t read_bytes       = read(pfd_map.at(cgi::Cgi::READ), buffer, BUF_SIZE);
			response                 = cgi.AddAndGetResponse(std::string(buffer, read_bytes));
		}
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug(response.response);
	return EXIT_SUCCESS;
}

/* ErrorTest: exec /cgi-bin/not_exist */
int Test4() {
	// request
	CgiRequest cgi_request;
	cgi_request.body_message                   = "test test";
	cgi_request.meta_variables[CONTENT_LENGTH] = utils::ToString(cgi_request.body_message.length());
	cgi_request.meta_variables[CONTENT_TYPE]   = "text/plain";
	cgi_request.meta_variables[PATH_INFO]      = "/aa/b";
	cgi_request.meta_variables[PATH_TRANSLATED] =
		html_dir_path + cgi_request.meta_variables.at(PATH_INFO);
	cgi_request.meta_variables[REQUEST_METHOD]  = "POST";
	cgi_request.meta_variables[SCRIPT_NAME]     = cgi_bin_dir_path + "/not_exist";
	cgi_request.meta_variables[SERVER_NAME]     = "localhost";
	cgi_request.meta_variables[SERVER_PORT]     = "8080";
	cgi_request.meta_variables[SERVER_PROTOCOL] = "HTTP/1.1";

	CgiResponse response;
	try {
		Cgi    cgi(cgi_request);
		PFdMap pfd_map = cgi.Run();
		if (cgi.IsWriteRequired()) {
			write(
				pfd_map.at(cgi::Cgi::WRITE),
				cgi_request.body_message.c_str(),
				cgi_request.body_message.length()
			);
		}
		while (!response.is_response_complete) {
			char    buffer[BUF_SIZE] = {};
			ssize_t read_bytes       = read(pfd_map.at(cgi::Cgi::READ), buffer, BUF_SIZE);
			response                 = cgi.AddAndGetResponse(std::string(buffer, read_bytes));
		}
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug(response.response);
	return EXIT_SUCCESS;
}

} // namespace

int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();
	ret |= Test2();
	ret |= Test3();
	ret |= Test4();

	return ret;
}
