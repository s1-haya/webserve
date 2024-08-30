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

	// http_method/expected
	// CF:   exist target resourse file
	// CRLF: use default status code file

	// ファイルが存在する場合
	std::string file;
	std::string expected_file = LoadFileContent("expected/file.txt");
	http::HttpResponse::GetHandler("test/file.txt", file);
	ret_code |= HandleResult(file, expected_file);

	// ファイルが存在しない場合
	std::string not_found;
	std::string expected_not_found = LoadFileContent("expected/not_found.txt");
	http::HttpResponse::GetHandler("test/a", not_found);
	ret_code |= HandleResult(not_found, expected_not_found);

	// ディレクトリの場合かつ'/'がない場合
	std::string redirect;
	std::string expected_redirect = LoadFileContent("expected/redirect.txt");
	http::HttpResponse::GetHandler("test/directory", redirect);
	ret_code |= HandleResult(redirect, expected_redirect);

	// ファイルが権限ない場合
	std::string forbidden;
	std::string expected_forbidden = LoadFileContent("expected/forbidden.txt");
	http::HttpResponse::GetHandler("test/no_authority_file", forbidden);
	ret_code |= HandleResult(forbidden, expected_forbidden);

	// POST test
	// 新しいファイルをアップロードする場合
	std::string test1_request_body_message = "OK";
	std::string test1_response_body_message;
	std::string expected_created = LoadFileContent("expected/created.txt");
	http::HttpResponse::PostHandler(
		"ok.txt", test1_request_body_message, test1_response_body_message
	);
	ret_code |= HandleResult(test1_response_body_message, expected_created);

	// すでにアップロードされたファイルをアップロードする場合
	std::string test2_request_body_message = "OK";
	std::string test2_response_body_message;
	std::string expected_no_content = LoadFileContent("expected/no_content.txt");
	http::HttpResponse::PostHandler(
		"ok.txt", test2_request_body_message, test2_response_body_message
	);
	ret_code |= HandleResult(test2_response_body_message, expected_no_content);

	// ディレクトリの場合
	std::string test3_request_body_message;
	std::string test3_response_body_message;
	http::HttpResponse::PostHandler(
		"test/directory", test3_request_body_message, test3_response_body_message
	);
	ret_code |= HandleResult(test3_response_body_message, expected_forbidden);

	// DELETE test
	// ファイルが存在するかつ親ディレクトリが書き込み権限あるとき
	std::string delete_test1_response_body_message;
	http::HttpResponse::DeleteHandler("ok.txt", delete_test1_response_body_message);
	ret_code |= HandleResult(delete_test1_response_body_message, expected_no_content);

	// ファイルが存在しない場合
	std::string delete_test2_response_body_message;
	http::HttpResponse::DeleteHandler("not_found.txt", delete_test2_response_body_message);
	ret_code |= HandleResult(delete_test2_response_body_message, expected_not_found);

	// ディレクトリ内にファイルが存在してる場合
	std::string delete_test3_response_body_message;
	http::HttpResponse::DeleteHandler("test", delete_test3_response_body_message);
	ret_code |= HandleResult(delete_test3_response_body_message, expected_forbidden);

	// 　ディレクトリ内にファイルが存在してない場合
	std::string delete_test4_response_body_message;
	http::HttpResponse::DeleteHandler("test/s", delete_test4_response_body_message);
	ret_code |= HandleResult(delete_test4_response_body_message, expected_forbidden);

	// 存在しないディレクトリの場合
	std::string delete_test5_response_body_message;
	http::HttpResponse::DeleteHandler("not_found_directory", delete_test5_response_body_message);
	ret_code |= HandleResult(delete_test5_response_body_message, expected_not_found);

	// 書き込み権限がないディレクトリの中にあるファイル場合
	std::string delete_test6_response_body_message;
	http::HttpResponse::DeleteHandler(
		"test/no_authority_directory/test.txt", delete_test6_response_body_message
	);
	ret_code |= HandleResult(delete_test6_response_body_message, expected_forbidden);
	return ret_code;
}
