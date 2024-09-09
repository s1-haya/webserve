#include "cgi_parse.hpp"
#include "http_format.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

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
		throw std::logic_error(std::string("Error at ") + file + ":" + ss.str());
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

// cgi_parseから見た相対パス
const std::string html_dir_path    = "../../../../html";
const std::string cgi_bin_dir_path = "../../../../cgi-bin";

int Test1() {
	// request
	const RequestLine request_line = {"GET", "/", "HTTP/1.1"};
	HttpRequestFormat request;
	request.request_line                  = request_line;
	request.header_fields[HOST]           = "host1";
	request.header_fields[CONNECTION]     = "keep-alive";
	request.header_fields[CONTENT_LENGTH] = "0";
	request.header_fields[CONTENT_TYPE]   = "text/plain";

	std::string cgi_script    = "/test.cgi";
	std::string cgi_path_info = "/aa/b";
	std::string cgi_extension = ".cgi";
	std::string server_port   = "8080";

	utils::Result<cgi::CgiRequest> parse_result =
		cgi::CgiParse::Parse(request, cgi_script + cgi_path_info, cgi_extension, server_port);
	cgi::MetaMap meta_variables = parse_result.GetValue().meta_variables;

	try {
		COMPARE(meta_variables.at("CONTENT_LENGTH"), request.header_fields.at("Content-Length"));
		COMPARE(meta_variables.at("CONTENT_TYPE"), request.header_fields.at("Content-Type"));
		COMPARE(meta_variables.at("PATH_INFO"), cgi_path_info);
		COMPARE(meta_variables.at("PATH_TRANSLATED"), html_dir_path + cgi_path_info);
		COMPARE(meta_variables.at("REQUEST_METHOD"), request_line.method);
		COMPARE(meta_variables.at("SCRIPT_NAME"), cgi_bin_dir_path + cgi_script);
		COMPARE(meta_variables.at("SERVER_NAME"), request.header_fields.at("Host"));
		COMPARE(meta_variables.at("SERVER_PORT"), server_port);
		COMPARE(meta_variables.at("SERVER_PROTOCOL"), request_line.version);
	} catch (const std::exception &e) {
		PrintNg();
		std::cerr << e.what() << '\n';
		return EXIT_FAILURE;
	}
	PrintOk();
	return EXIT_SUCCESS;
}

} // namespace

int main() {
	int ret = EXIT_SUCCESS;

	ret |= Test1();

	return ret;
}
