#include "cgi.hpp"
#include "cgi_parse.hpp"
#include "http_format.hpp"
#include "http_message.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

namespace {

using namespace http::cgi;

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

// template <typename T>
// void IsSame(const T &a, const T &b, const char *file, int line) {
// 	if (a != b) {
// 		std::stringstream ss;
// 		ss << line;
// 		throw std::logic_error(
// 			std::string("Error at ") + file + ":" + ss.str() + " - Expected: " + std::string(b) +
// 			", Got: " + std::string(a)
// 		);
// 	}
// }

// /**
//  * @brief Output where is the error
//  */
// #define COMPARE(a, b) IsSame(a, b, __FILE__, __LINE__)

// template <class InputIt>
// InputIt Next(InputIt it, typename std::iterator_traits<InputIt>::difference_type n = 1) {
// 	std::advance(it, n);
// 	return it;
// } // std::next for c++98

// ================================================= //

// cgi_parseから見た相対パス
const std::string html_dir_path    = "../../../html";
const std::string cgi_bin_dir_path = "../../../cgi-bin";

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

	std::string response_body_message;
	try {
		Cgi cgi(cgi_request);
		cgi.Run(response_body_message);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug(response_body_message);
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

	std::string response_body_message;
	try {
		Cgi cgi(cgi_request);
		cgi.Run(response_body_message);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	utils::Debug(response_body_message);
	return EXIT_SUCCESS;
}

} // namespace

int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();
	ret |= Test2();

	return ret;
}
