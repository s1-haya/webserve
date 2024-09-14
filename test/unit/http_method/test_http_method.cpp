#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_method.hpp"
#include "http_response.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <ctime>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

namespace {

struct MethodArgument {
	MethodArgument(
		const std::string                &path,
		const std::string                &method,
		const http::Method::AllowMethods &allow_methods,
		const std::string                &request_body_message,
		std::string                      &response_body_message,
		http::HeaderFields               &response_header_fields,
		const std::string                &index_file_path = "",
		bool                              autoindex_on    = false
	)
		: path(path),
		  method(method),
		  allow_methods(allow_methods),
		  request_body_message(request_body_message),
		  response_body_message(response_body_message),
		  response_header_fields(response_header_fields),
		  index_file_path(index_file_path),
		  autoindex_on(autoindex_on) {
		response_body_message.clear();
	}
	const std::string                &path;
	const std::string                &method;
	const http::Method::AllowMethods &allow_methods;
	const std::string                &request_body_message;
	std::string                      &response_body_message;
	http::HeaderFields               &response_header_fields;
	const std::string                &index_file_path;
	bool                              autoindex_on;
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

std::string CreateAutoIndexContent(const std::string &path) {
	DIR        *dir = opendir(path.c_str());
	std::string content;

	struct dirent *entry;
	content += "<html>\n"
			   "<head><title>Index of /</title></head>\n"
			   "<body><h1>Index of /</h1><hr><pre>"
			   "<a href=\"../\">../</a>\n";
	while ((entry = readdir(dir)) != NULL) {
		std::string full_path = path + "/" + entry->d_name;
		struct stat file_stat;
		if (stat(full_path.c_str(), &file_stat) == 0) {
			content += "<a href=\"" + std::string(entry->d_name) + "\">" +
					   std::string(entry->d_name) + "</a> ";
			content += utils::ToString(file_stat.st_size) + " bytes ";
			content += std::ctime(&file_stat.st_mtime);
		} else {
			content += "<a href=\"" + std::string(entry->d_name) + "\">" +
					   std::string(entry->d_name) + "</a> ";
			content += "Error getting file stats\n";
		}
	}
	content += "</pre><hr></body></html>";
	closedir(dir);

	return content;
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
			srcs.response_body_message,
			srcs.response_header_fields,
			srcs.index_file_path,
			srcs.autoindex_on
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
	http::Method::AllowMethods allow_methods;
	allow_methods.push_back(http::GET);
	allow_methods.push_back(http::POST);
	allow_methods.push_back(http::DELETE);
	std::string        request;
	std::string        response;
	http::HeaderFields response_header_fields;

	// http_method/expected
	// LF:   exist target resourse file
	std::string expected_file       = LoadFileContent("expected/file.txt");
	std::string expected_index_file = LoadFileContent("expected/index.txt");
	std::string expected_autoindex  = CreateAutoIndexContent("test/");
	// CRLF: use default status code file
	std::string expected_created    = LoadFileContent("expected/created.txt");
	std::string expected_no_content = LoadFileContent("expected/no_content.txt");
	std::string expected_redirect   = LoadFileContent("expected/redirect.txt");
	std::string expected_forbidden  = LoadFileContent("expected/forbidden.txt");
	std::string expected_not_found  = LoadFileContent("expected/not_found.txt");

	// GET test
	// ファイルが存在する場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/file.txt", http::GET, allow_methods, request, response, response_header_fields
		),
		expected_file
	);

	// ファイルが存在しない場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/a", http::GET, allow_methods, request, response, response_header_fields
		),
		expected_not_found
	);

	// ディレクトリの場合かつ'/'がない場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/directory", http::GET, allow_methods, request, response, response_header_fields
		),
		expected_redirect
	);

	// ファイルが権限ない場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/no_authority_file",
			http::GET,
			allow_methods,
			request,
			response,
			response_header_fields
		),
		expected_forbidden
	);

	// ディレクトリで'/'があり、indexがある場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/",
			http::GET,
			allow_methods,
			request,
			response,
			response_header_fields,
			"index.txt"
		),
		expected_index_file
	);

	// ディレクトリで'/'があり、autoindexがある場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/", http::GET, allow_methods, request, response, response_header_fields, "", true
		),
		expected_autoindex
	);

	// POST test
	// 新しいファイルをアップロードする場合
	const std::string &post_test1_request_body_message = "OK";
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"ok.txt",
			http::POST,
			allow_methods,
			post_test1_request_body_message,
			response,
			response_header_fields
		),
		expected_created
	);

	// すでにアップロードされたファイルをアップロードする場合
	const std::string &post_test2_request_body_message = "OK";
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"ok.txt",
			http::POST,
			allow_methods,
			post_test2_request_body_message,
			response,
			response_header_fields
		),
		expected_no_content
	);

	// ディレクトリの場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/directory", http::POST, allow_methods, request, response, response_header_fields
		),
		expected_forbidden
	);

	// DELETE test
	// ファイルが存在するかつ親ディレクトリが書き込み権限あるとき
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"ok.txt", http::DELETE, allow_methods, request, response, response_header_fields
		),
		expected_no_content
	);

	// ファイルが存在しない場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"not_found.txt", http::DELETE, allow_methods, request, response, response_header_fields
		),
		expected_not_found
	);

	// ディレクトリ内にファイルが存在してる場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test", http::DELETE, allow_methods, request, response, response_header_fields
		),
		expected_forbidden
	);

	// ディレクトリ内にファイルが存在してない場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/s", http::DELETE, allow_methods, request, response, response_header_fields
		),
		expected_forbidden
	);

	// 存在しないディレクトリの場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"not_found_directory",
			http::DELETE,
			allow_methods,
			request,
			response,
			response_header_fields
		),
		expected_not_found
	);

	// 書き込み権限がないディレクトリの中にあるファイル場合
	ret_code |= MethodHandlerResult(
		MethodArgument(
			"test/no_authority_directory/test.txt",
			http::DELETE,
			allow_methods,
			request,
			response,
			response_header_fields
		),
		expected_forbidden
	);
	return ret_code;
}
