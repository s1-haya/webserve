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
	http::RequestLine       expect2("", "/", "HTTP/1.1");
	http::HttpRequestResult test2 = http::HttpParse::Run("GEdT / HTTP/1.1");
	assert(http::BAD_REQUEST == test2.status_code);

	// methodがUS-ASCII以外の文字が含まれてる
	http::RequestLine       expect3("", "/", "HTTP/1.1");
	http::HttpRequestResult test3 = http::HttpParse::Run("GEあ / HTTP/1.1");
	assert(http::BAD_REQUEST == test3.status_code);

	// 課題要件以外のmethodが含まれてる
	http::RequestLine       expect4("", "/", "HTTP/1.1");
	http::HttpRequestResult test4 = http::HttpParse::Run("HEAD / HTTP/1.1");
	assert(http::NOT_IMPLEMENTED == test4.status_code);

	// header_fields: 成功例
	http::RequestLine       expect5("GET", "/", "HTTP/1.1");
	http::HttpRequestResult test5 = http::HttpParse::Run(
		"GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n"
	);
	assert(http::OK == test5.status_code);
	assert_request_line(expect5, test5.request.request_line);
	assert("www.example.com" == test5.request.header_fields["Host"]);

	// header_fields: セミコロン以降に複数OWS(SpaceとHorizontal Tab)が設定されてる場合
	http::RequestLine       expect6("GET", "/", "HTTP/1.1");
	http::HttpRequestResult test6 = http::HttpParse::Run(
		"GET / HTTP/1.1\r\nHost:    \twww.example.com\r\nConnection: keep-alive\r\n\r\n"
	);
	assert(http::OK == test6.status_code);
	assert_request_line(expect6, test6.request.request_line);
	assert("www.example.com" == test6.request.header_fields["Host"]);

	// header_fields: 存在しないfield_nameの場合
	http::RequestLine       expect7("", "/", "HTTP/1.1");
	http::HttpRequestResult test7 = http::HttpParse::Run(
		"GET / HTTP/1.1\r\nHosdt: www.example.com\r\nConnection: keep-alive\r\n\r\n"
	);
	assert(http::BAD_REQUEST == test7.status_code);

	// // RequestTargetが絶対パスじゃない
	// http::RequestLine expect6("GET", "400", "HTTP/1.1");
	// http::HttpRequestResult test5 = a.Run("GET index.html/ HTTP/1.1");
	// assert_request_line(expect5, test5.request_line);

	// // HTTPバージョンが異なる
	// http::RequestLine expect6("GET", "/", "400");
	// http::HttpRequestResult test6 = a.Run("GET / HTTP/1.0");
	// assert_request_line(expect6, test6.request_line);

	// StatusLineとHeadersを含むHTTPリクエスト
	// http::HttpRequestFormat test2 =
	// 	a.Run("GET /sub HTTP/1.1\r\nHost: www.example.com\r\nConnection: keep-alive\r\n\r\n");
	// assert("GET" == test2.request_line.method);
	// assert("/sub" == test2.request_line.request_target);
	// assert("HTTP/1.1" == test2.request_line.version);
	// assert("www.example.com" == test2.header_fields["Host"]);
	// assert("keep-alive" == test2.header_fields["Connection"]);

	// // StatusLineとHeader、Bodyを含むHTTPリクエスト
	// http::HttpRequestFormat test3 = http::HttpParse::Run("POST /index.html HTTP/1.1\r\nHost: "
	// 								"www.example.com\r\nConnection: keep-alive\r\n\r\nbodymessage");
	// assert("POST" == test3.request_line.method);
	// assert("/index.html" == test3.request_line.request_target);
	// assert("HTTP/1.1" == test3.request_line.version);
	// assert("www.example.com" == test3.header_fields["Host"]);
	// assert("keep-alive" == test3.header_fields["Connection"]);
	// assert("bodymessage" == test3.message_body);

	// // StatusLineとHeader、Body(改行バージョン)を含むHTTPリクエスト
	// http::HttpRequestFormat test4 =
	// 	a.Run("POST /index.html HTTP/1.1\r\nHost: www.example.com\r\nConnection: "
	// 		  "keep-alive\r\n\r\naa\r\naa\naa");
	// std::cout << "Method: " << test4.request_line.method << std::endl;
	// std::cout << "request_target: " << test4.request_line.request_target << std::endl;
	// std::cout << "version: " << test4.request_line.version << std::endl;
	return 0;
}
