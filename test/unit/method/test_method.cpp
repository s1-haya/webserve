#include "http_response.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace {

std::string LoadFileContent(const std::string &file_path) {
	std::ifstream file(file_path.c_str());
	if (!file) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		return "";
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	return file_content.str();
}

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

template <typename T>
int HandleResult(const T &result, const T &expected) {
	if (result == expected) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		return EXIT_FAILURE;
	}
}
} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;

	// GET test
	// CF -> success or error_pages directive
	// CRLF -> default error file

	// CF
	std::string makefile;
	std::string expected_makefile = LoadFileContent("expected/makefile.txt");
	http::HttpResponse::GetHandler("Makefile", makefile);
	ret_code = HandleResult(makefile, expected_makefile);

	// CRLF
	std::string not_found;
	std::string expected_not_found = LoadFileContent("expected/not_found.txt");
	http::HttpResponse::GetHandler("test/a", not_found);
	ret_code = HandleResult(not_found, expected_not_found);

	// CRLF
	std::string redirect;
	std::string expected_redirect = LoadFileContent("expected/redirect.txt");
	http::HttpResponse::GetHandler("test/directory", redirect);
	ret_code = HandleResult(redirect, expected_redirect);

	// CRLF 権限のないファイルをaddすることができなかったためローカルで各自テストしてください
	// std::string forbidden;
	// std::string expected_forbidden = LoadFileContent("expected/forbidden.txt");
	// http::HttpResponse::GetHandler("test/forbidden_file", forbidden);
	// ret_code = HandleResult(forbidden, expected_forbidden);

	// POST test
	// 新しいファイルをアップロードする場合
	std::string test1_request_body_message = "OK";
	std::string test1_response_body_message;
	std::string expected_created = LoadFileContent("expected/created.txt");
	http::HttpResponse::PostHandler(
		"ok.txt", test1_request_body_message, test1_response_body_message
	);
	ret_code = HandleResult(test1_response_body_message, expected_created);

	// すでにアップロードされたファイルをアップロードする場合
	std::string test2_request_body_message = "OK";
	std::string test2_response_body_message;
	std::string expected_no_content = LoadFileContent("expected/no_content.txt");
	http::HttpResponse::PostHandler(
		"ok.txt", test2_request_body_message, test2_response_body_message
	);
	ret_code = HandleResult(test2_response_body_message, expected_no_content);

	// リソースがディレクトリの場合
	std::string test3_request_body_message = "OK";
	std::string test3_response_body_message;
	std::string expected_forbidden = LoadFileContent("expected/forbidden.txt");
	http::HttpResponse::PostHandler(
		"test/directory", test3_request_body_message, test3_response_body_message
	);
	ret_code = HandleResult(test3_response_body_message, expected_forbidden);

	// DELETE test
	// ファイルが存在するかつ親ディレクトリが書き込み権限あるとき
	std::string delete_test1_response_body_message;
	http::HttpResponse::DeleteHandler("ok.txt", delete_test1_response_body_message);
	ret_code = HandleResult(delete_test1_response_body_message, expected_no_content);
	return ret_code;
}
