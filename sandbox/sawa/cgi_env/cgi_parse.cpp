#include "cgi_parse.hpp"

namespace cgi {

CGI::MetaMap create_request_meta_variables() {
	CGI::MetaMap request_meta_variables;
	request_meta_variables["AUTH_TYPE"];
	request_meta_variables["CONTENT_LENGTH"] = "26";
	request_meta_variables["CONTENT_TYPE"];
	request_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	request_meta_variables["PATH_INFO"];
	request_meta_variables["PATH_TRANSLATED"];
	request_meta_variables["QUERY_STRING"];
	request_meta_variables["REMOTE_ADDR"];
	request_meta_variables["REMOTE_HOST"];
	request_meta_variables["REMOTE_IDENT"];
	request_meta_variables["REMOTE_USER"];
	request_meta_variables["REQUEST_METHOD"]  = "POST";
	request_meta_variables["SCRIPT_NAME"]     = "../../../test/apache/cgi/print_stdin.pl";
	request_meta_variables["SERVER_NAME"]     = "localhost";
	request_meta_variables["SERVER_PORT"]     = "8080";
	request_meta_variables["SERVER_PROTOCOL"] = "HTTP/1.1";
	request_meta_variables["SERVER_SOFTWARE"] = "Webserv/1.1";
	return request_meta_variables;
}

// HTTPリクエストをパースしてCGIRequestを生成
CGIRequest CGIParse::parse(const std::string &http_request) {
	CGIRequest         request;
	std::istringstream request_stream(http_request);
	std::string        line;

	// リクエストラインのパース
	if (std::getline(request_stream, line)) {
		std::istringstream line_stream(line);
		line_stream >> request.meta_variables["REQUEST_METHOD"]; // メソッド
		line_stream >> request.meta_variables["SCRIPT_NAME"];    // スクリプト名（パス）
		if (request.meta_variables["SCRIPT_NAME"].find('?') != std::string::npos) {
			size_t pos = request.meta_variables["SCRIPT_NAME"].find('?');
			request.meta_variables["QUERY_STRING"] =
				request.meta_variables["SCRIPT_NAME"].substr(pos + 1);
			request.meta_variables["SCRIPT_NAME"] =
				request.meta_variables["SCRIPT_NAME"].substr(0, pos);
		}
	}

	// ヘッダーセクションのパース
	std::string header_section;
	while (std::getline(request_stream, line) && line != "\r") {
		header_section += line + "\n";
	}
	std::cout << header_section << std::endl;
	// request.headers = parse_headers(header_section);

	// コンテンツ情報の設定
	request.meta_variables["CONTENT_LENGTH"] =
		get_value_from_header(header_section, "Content-Length");
	request.meta_variables["CONTENT_TYPE"] = get_value_from_header(header_section, "Content-Type");

	// ボディのパース
	if (request.meta_variables["REQUEST_METHOD"] == "POST" &&
		!request.meta_variables["CONTENT_LENGTH"].empty()) {
		int         content_length = std::stoi(request.meta_variables["CONTENT_LENGTH"]);
		std::string body(content_length, '\0');
		request_stream.read(&body[0], content_length);
		request.body_message = body;
	}

	return request;
}

// ヘッダーをパースしてマップに変換
std::map<std::string, std::string> CGIParse::parse_headers(const std::string &header_section) {
	std::map<std::string, std::string> headers;
	std::istringstream                 header_stream(header_section);
	std::string                        line;

	while (std::getline(header_stream, line)) {
		size_t pos = line.find(':');
		if (pos != std::string::npos) {
			std::string key   = line.substr(0, pos);
			std::string value = line.substr(pos + 1);
			headers[key]      = value;
		}
	}

	return headers;
}

// ヘッダーから特定の値を取得
std::string CGIParse::get_value_from_header(const std::string &header, const std::string &key) {
	size_t pos = header.find(key + ":");
	if (pos != std::string::npos) {
		size_t start = pos + key.length() + 1;
		size_t end   = header.find("\n", start);
		return header.substr(start, end - start);
	}
	return "";
}

} // namespace cgi

// テスト用のメイン関数
// int main() {
// 	std::string http_request = "POST /cgi-bin/print_stdin.pl HTTP/1.1\r\n"
// 							   "Host: localhost:8080\r\n"
// 							   "Server: Apache/2.4.59 (Unix)\r\n"
// 							   "Content-Type: application/x-www-form-urlencoded\r\n"
// 							   "Content-Length: 26\r\n"
// 							   "WWW-Authenticate: Basic realm=\" Basic Auth \"6\r\n"
// 							   "\r\n"
// 							   "name=ChatGPT&message=Hello";

// 	cgi_parse::CGIParse   parser;
// 	cgi_parse::CGIRequest request = parser.parse(http_request);

// 	// パース結果の表示
// 	std::cout << "Method: " << request.meta_variables["REQUEST_METHOD"] << std::endl;
// 	std::cout << "Script Name: " << request.meta_variables["SCRIPT_NAME"] << std::endl;
// 	std::cout << "Query String: " << request.meta_variables["QUERY_STRING"] << std::endl;
// 	std::cout << "Content Type: " << request.meta_variables["CONTENT_TYPE"] << std::endl;
// 	std::cout << "Content Length: " << request.meta_variables["CONTENT_LENGTH"] << std::endl;
// 	std::cout << "Post Data: " << request.body_message << std::endl;

// 	return 0;
// }
