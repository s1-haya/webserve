#include "http_parse.hpp"
#include <cassert>
#include <iostream>

int main(void) {
	// StatusLineのみのHTTPリクエスト
	http::HttpRequest test1 = http::HttpParse::Run("GET / HTTP/1.1");
	assert("GET" == test1.status_line.method);
	assert("/" == test1.status_line.uri);
	assert("HTTP/1.1" == test1.status_line.version);

	// StatusLineとHeadersを含むHTTPリクエスト
	http::HttpRequest test2 =
		 http::HttpParse::Run("GET /sub HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n");
	assert("GET" == test2.status_line.method);
	assert("/sub" == test2.status_line.uri);
	assert("HTTP/1.1" == test2.status_line.version);
	assert("www.example.com" == test2.header_fields["Host"]);
	assert("keep-alive" == test2.header_fields["Connection"]);

	// StatusLineとHeader、Bodyを含むHTTPリクエスト
	http::HttpRequest test3 =  http::HttpParse::Run("POST /index.html HTTP/1.1\r\nHost: "
									"www.example.com\r\nConnection: keep-alive\r\n\r\nbodymessage");
	assert("POST" == test3.status_line.method);
	assert("/index.html" == test3.status_line.uri);
	assert("HTTP/1.1" == test3.status_line.version);
	assert("www.example.com" == test3.header_fields["Host"]);
	assert("keep-alive" == test3.header_fields["Connection"]);

	// StatusLineとHeader、Body(改行バージョン)を含むHTTPリクエスト
	http::HttpRequest test4 =
		 http::HttpParse::Run("POST /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: "
			  "keep-alive\r\n\r\naa\r\naa\naa");
	// std::cout << "Method: " << test4.status_line.method << std::endl;
	// std::cout << "uri: " << test4.status_line.uri << std::endl;
	// std::cout << "version: " << test4.status_line.version << std::endl;
	return 0;
}
