#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_method.hpp"
#include "http_response.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace {

typedef std::list<std::string> AllowMethods;

struct MethodArgument {
	MethodArgument(
		const std::string  &path,
		const std::string  &method,
		const AllowMethods &allow_methods,
		const std::string  &request_body_message,
		std::string        &response_body_message
	)
		: path(path),
		  method(method),
		  allow_methods(allow_methods),
		  request_body_message(request_body_message),
		  response_body_message(response_body_message) {}
	const std::string  &path;
	const std::string  &method;
	const AllowMethods &allow_methods;
	const std::string  &request_body_message;
	std::string        &response_body_message;
};

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

int MethodHandlerResult(const MethodArgument &srcs, const std::string &expected_body_message) {
	int result = 0;
	try {
		http::Method::Handler(
			srcs.path,
			srcs.method,
			srcs.allow_methods,
			srcs.request_body_message,
			srcs.response_body_message
		);
		result = HandleResult(srcs.response_body_message, expected_body_message);

	} catch (const http::HttpException &e) {
		srcs.response_body_message =
			http::HttpResponse::CreateDefaultBodyMessageFormat(e.GetStatusCode());
		result = HandleResult(srcs.response_body_message, expected_body_message);
		std::cerr << utils::color::GRAY << e.what() << utils::color::RESET << std::endl;
	}
	return result;
}

} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;

	// method test init
	AllowMethods allow_methods;
	allow_methods.push_back(http::GET);
	allow_methods.push_back(http::POST);
	allow_methods.push_back(http::DELETE);
	std::string request;
	std::string response;

	// http_method/expected
	// LF:   exist target resourse file
	std::string expected_file = LoadFileContent("expected/file.txt");
	// CRLF: use default status code file
	std::string expected_created    = LoadFileContent("expected/created.txt");
	std::string expected_no_content = LoadFileContent("expected/no_content.txt");
	std::string expected_redirect   = LoadFileContent("expected/redirect.txt");
	std::string expected_forbidden  = LoadFileContent("expected/forbidden.txt");
	std::string expected_not_found  = LoadFileContent("expected/not_found.txt");

	// GET test
	// ファイルが存在する場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test/file.txt", http::GET, allow_methods, request, response), expected_file
	);

	// ファイルが存在しない場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test/a", http::GET, allow_methods, request, response), expected_not_found
	);

	// ディレクトリの場合かつ'/'がない場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test/directory", http::GET, allow_methods, request, response),
		expected_redirect
	);

	// ファイルが権限ない場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test/no_authority_file", http::GET, allow_methods, request, response),
		expected_forbidden
	);

	// POST test
	// 新しいファイルをアップロードする場合
	const std::string &post_test1_request_body_message = "OK";
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"ok.txt", http::POST, allow_methods, post_test1_request_body_message, response
		),
		expected_created
	);

	// すでにアップロードされたファイルをアップロードする場合
	const std::string &post_test2_request_body_message = "OK";
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"ok.txt", http::POST, allow_methods, post_test2_request_body_message, response
		),
		expected_no_content
	);

	// ディレクトリの場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test/directory", http::POST, allow_methods, request, response),
		expected_forbidden
	);

	// DELETE test
	// ファイルが存在するかつ親ディレクトリが書き込み権限あるとき
	ret_code |= MethodHandlerResult(
		MethodArgument("ok.txt", http::DELETE, allow_methods, request, response),
		expected_no_content
	);

	// ファイルが存在しない場合
	ret_code |= MethodHandlerResult(
		MethodArgument("not_found.txt", http::DELETE, allow_methods, request, response),
		expected_not_found
	);

	// ディレクトリ内にファイルが存在してる場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test", http::DELETE, allow_methods, request, response), expected_forbidden
	);

	// ディレクトリ内にファイルが存在してない場合
	ret_code |= MethodHandlerResult(
		MethodArgument("test/s", http::DELETE, allow_methods, request, response), expected_forbidden
	);

	// 存在しないディレクトリの場合
	ret_code |= MethodHandlerResult(
		MethodArgument("not_found_directory", http::DELETE, allow_methods, request, response),
		expected_not_found
	);

	// 書き込み権限がないディレクトリの中にあるファイル場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/no_authority_directory/test.txt", http::DELETE, allow_methods, request, response
		),
		expected_forbidden
	);
	return ret_code;
}
