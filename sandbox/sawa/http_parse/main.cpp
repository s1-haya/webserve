#include "http_parse.hpp"
#include <cassert>
#include <iostream>

void assert_request_line(const http::RequestLine &expect, const http::RequestLine &res) {
	assert(expect.method == res.method);
	assert(expect.uri == res.uri);
	assert(expect.version == res.version);
}

int main(void) {
	http::HTTPParse a;

	// StatusLineのみのHTTPリクエスト
	http::RequestLine expect1("GET", "html/index.html", "HTTP/1.1");
	http::HTTPRequest test1 = a.Run("GET / HTTP/1.1");
	assert_request_line(expect1, test1.request_line);

	// methodが小文字
	http::RequestLine expect5("400", "html/index.html", "HTTP/1.1");
	http::HTTPRequest test5 = a.Run("GEdT / HTTP/1.1");
	assert_request_line(expect5, test5.request_line);

	// StatusLineとHeadersを含むHTTPリクエスト
	http::HTTPRequest test2 =
		a.Run("GET /sub HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n");
	assert("GET" == test2.request_line.method);
	assert("html/sub/index.html" == test2.request_line.uri);
	assert("HTTP/1.1" == test2.request_line.version);
	assert("www.example.com" == test2.header_fields["Host"]);
	assert("keep-alive" == test2.header_fields["Connection"]);

	// StatusLineとHeader、Bodyを含むHTTPリクエスト
	http::HTTPRequest test3 = a.Run("POST /index.html HTTP/1.1\r\nHost: "
									"www.example.com\r\nConnection: keep-alive\r\n\r\nbodymessage");
	assert("POST" == test3.request_line.method);
	assert("html/index.html/index.html" == test3.request_line.uri);
	assert("HTTP/1.1" == test3.request_line.version);
	assert("www.example.com" == test3.header_fields["Host"]);
	assert("keep-alive" == test3.header_fields["Connection"]);
	assert("bodymessage" == test3.message_body);

	// StatusLineとHeader、Body(改行バージョン)を含むHTTPリクエスト
	http::HTTPRequest test4 =
		a.Run("POST /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: "
			  "keep-alive\r\n\r\naa\r\naa\naa");
	// std::cout << "Method: " << test4.request_line.method << std::endl;
	// std::cout << "uri: " << test4.request_line.uri << std::endl;
	// std::cout << "version: " << test4.request_line.version << std::endl;
	return 0;
}
