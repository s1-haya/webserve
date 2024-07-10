#include "http_parse.hpp"
#include <iostream>

int main(void) {
	http::HTTPParse a;

	// StatusLineのみのHTTPリクエスト
	http::HTTPRequest test1 = a.Run("GET / HTTP1.1");
	// std::cout << "Method: " << test1.status_line.method << std::endl;
	// std::cout << "uri: " << test1.status_line.uri << std::endl;
	// std::cout << "version: " << test1.status_line.version << std::endl;

	// StatusLineとHeadersを含むHTTPリクエスト
	http::HTTPRequest test2 =
		a.Run("GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n"
		);
	// std::cout << "Method: " << test2.status_line.method << std::endl;
	// std::cout << "uri: " << test2.status_line.uri << std::endl;
	// std::cout << "version: " << test2.status_line.version << std::endl;

	// StatusLineとHeader、Bodyを含むHTTPリクエスト
	http::HTTPRequest test3 = a.Run(
		"GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\naaaaaa"
	);
	// std::cout << "Method: " << test3.status_line.method << std::endl;
	// std::cout << "uri: " << test3.status_line.uri << std::endl;
	// std::cout << "version: " << test3.status_line.version << std::endl;

	// StatusLineとHeader、Body(改行バージョン)を含むHTTPリクエスト
	http::HTTPRequest test4 =
		a.Run("GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: "
			  "keep-alive\r\n\r\naa\r\naa\naa");
	// std::cout << "Method: " << test4.status_line.method << std::endl;
	// std::cout << "uri: " << test4.status_line.uri << std::endl;
	// std::cout << "version: " << test4.status_line.version << std::endl;
	return 0;
}
