#include "http_parse.hpp"
#include <cassert>
#include <iostream>

void assert_request_line(const http::RequestLine &expect, const http::RequestLine &res) {
	assert(expect.method == res.method);
	assert(expect.request_target == res.request_target);
	assert(expect.version == res.version);
}

int main(void) {
	// todo: expectはエラーの場合はエラーのステータスコードを入れてます。enumを作成したら削除する

	// StatusLineのみのHTTPリクエスト
	http::RequestLine       expect1("GET", "/", "HTTP/1.1");
	http::HttpRequestResult test1 = http::HttpParse::Run("GET / HTTP/1.1");
	assert_request_line(expect1, test1.request.request_line);

	// methodが小文字が含まれてる
	http::RequestLine       expect2("400", "/", "HTTP/1.1");
	http::HttpRequestResult test2 = http::HttpParse::Run("GEdT / HTTP/1.1");
	assert_request_line(expect2, test2.request.request_line);

	// methodがUS-ASCII以外の文字が含まれてる
	http::RequestLine       expect3("400", "/", "HTTP/1.1");
	http::HttpRequestResult test3 = http::HttpParse::Run("GEあ / HTTP/1.1");
	assert_request_line(expect3, test3.request.request_line);

	// 課題要件以外のmethodが含まれてる
	http::RequestLine       expect4("501", "/", "HTTP/1.1");
	http::HttpRequestResult test4 = http::HttpParse::Run("HEAD / HTTP/1.1");
	assert_request_line(expect4, test4.request.request_line);

	// // RequestTargetが絶対パスじゃない
	// http::RequestLine expect5("GET", "400", "HTTP/1.1");
	// http::HttpRequestResult test5 = a.Run("GET index.html/ HTTP/1.1");
	// assert_request_line(expect5, test5.request_line);

	// // HTTPバージョンが異なる
	// http::RequestLine expect6("GET", "/", "400");
	// http::HttpRequestResult test6 = a.Run("GET / HTTP/1.0");
	// assert_request_line(expect6, test6.request_line);

	// StatusLineとHeadersを含むHTTPリクエスト
	// http::HttpRequest test2 =
	// 	a.Run("GET /sub HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n");
	// assert("GET" == test2.request_line.method);
	// assert("/sub" == test2.request_line.request_target);
	// assert("HTTP/1.1" == test2.request_line.version);
	// assert("www.example.com" == test2.header_fields["Host"]);
	// assert("keep-alive" == test2.header_fields["Connection"]);

	// // StatusLineとHeader、Bodyを含むHTTPリクエスト
	// http::HttpRequest test3 = http::HttpParse::Run("POST /index.html HTTP/1.1\r\nHost: "
	// 								"www.example.com\r\nConnection: keep-alive\r\n\r\nbodymessage");
	// assert("POST" == test3.request_line.method);
	// assert("/index.html" == test3.request_line.request_target);
	// assert("HTTP/1.1" == test3.request_line.version);
	// assert("www.example.com" == test3.header_fields["Host"]);
	// assert("keep-alive" == test3.header_fields["Connection"]);
	// assert("bodymessage" == test3.message_body);

	// // StatusLineとHeader、Body(改行バージョン)を含むHTTPリクエスト
	// http::HttpRequest test4 =
	// 	a.Run("POST /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: "
	// 		  "keep-alive\r\n\r\naa\r\naa\naa");
	// std::cout << "Method: " << test4.request_line.method << std::endl;
	// std::cout << "request_target: " << test4.request_line.request_target << std::endl;
	// std::cout << "version: " << test4.request_line.version << std::endl;
	return 0;
}
