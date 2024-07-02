#include "color.hpp"
#include <iostream>
#include <map>

// メタ変数を作成する関数
// RFC3875 https://datatracker.ietf.org/doc/html/rfc3875#section-4.1
// meta-variable-name = "AUTH_TYPE" | "CONTENT_LENGTH" | "CONTENT_TYPE" | "GATEWAY_INTERFACE" |
// 						 "PATH_INFO" | "PATH_TRANSLATED" | "QUERY_STRING" | "REMOTE_ADDR" |
// 						 "REMOTE_HOST" | "REMOTE_IDENT" | "REMOTE_USER" | "REQUEST_METHOD" |
// 						 "SCRIPT_NAME" | "SERVER_NAME" | "SERVER_PORT" | "SERVER_PROTOCOL" |
// 						 "SERVER_SOFTWARE"
std::map<std::string, std::string> create_request_meta_variables();

// メタ変数から環境変数を作成する関数
const char **create_cgi_env(const std::map<std::string, std::string> &request_meta_variables);

int main(void) {
	const std::string expected[] = {
		"AUTH_TYPE",
		"CONTENT_LENGTH",
		"CONTENT_TYPE",
		"GATEWAY_INTERFACE",
		"PATH_INFO",
		"PATH_TRANSLATED",
		"QUERY_STRING",
		"REMOTE_ADDR",
		"REMOTE_HOST",
		"REMOTE_IDENT",
		"REMOTE_USER",
		"REQUEST_METHOD",
		"SCRIPT_NAME",
		"SERVER_NAME",
		"SERVER_PORT",
		"SERVER_PROTOCOL",
		"SERVER_SOFTWARE"
	};
	std::map<std::string, std::string> request_meta_variables = create_request_meta_variables();
	const size_t                       expected_size = sizeof(expected) / sizeof(expected[0]);

	// メタ変数の値があるかどうか確認するテスト（expectedはRFC3875のメタ変数の値をコピペした）
	for (size_t i = 0; i < expected_size; ++i) {
		if (request_meta_variables.find(expected[i]) != request_meta_variables.end()) {
			std::cout << utils::color::GREEN << "[OK] " << utils::color::RESET << expected[i]
					  << std::endl;
		} else {
			std::cerr << utils::color::RED << "[NG] " << utils::color::RESET
					  << "Missing: " << expected[i] << std::endl;
		}
	}

	// 環境変数を出力する（=が付与されてるか、valueが紐づいてるかどうか、動的にメモリが確保できてるか）
	const char **cgi_env = create_cgi_env(request_meta_variables);
	for (size_t i = 0; cgi_env[i] != NULL; i++) {
		std::cout << cgi_env[i] << std::endl;
	}
	delete[] cgi_env;
	return (0);
}
