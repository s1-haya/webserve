#include "http.hpp"

int main(void) {
	int ret_code = 0;
	// // 10.ボディメッセージを追加で送信した場合
	// http::Http                  test4_body_message;
	// http::HttpRequestParsedData test4_expected_body_message;
	// test4_expected_body_message.request_result.status_code          = http::StatusCode(http::OK);
	// test4_expected_body_message.is_request_format.is_request_line   = true;
	// test4_expected_body_message.is_request_format.is_header_fields  = true;
	// test4_expected_body_message.is_request_format.is_body_message   = false;
	// test4_expected_body_message.request_result.request.body_message = "abc";

	// test4_body_message.ParseHttpRequestFormat(
	// 	1, "GET / HTTP/1.1\r\nHost: test\r\nContent-Length:  3\r\n\r\na"
	// );
	// Result result = IsSameHttpRequestParsedData(
	// 	test4_body_message.GetClientData(1), test4_expected_body_message
	// );
	// ret_code |= HandleResult(result);

	// // 11.追加でボディメッセージを送信
	// test4_body_message.ParseHttpRequestFormat(1, "bc");
	// test4_expected_body_message.is_request_format.is_body_message = true;
	// result                                                        = IsSameHttpRequestParsedData(
	//     test4_body_message.GetClientData(1), test4_expected_body_message
	// );
	// ret_code |= HandleResult(result);

	// todo: http/http_response/test_http_response.cpp responseを確認、実行、作成のテスト
	// リクエストのステータスコードが200の場合
	// http::Http test1_response;
	// test1_response.ParseHttpRequestFormat(
	// 	1, "GET / HTTP/1.1\r\nHost: test\r\nContent-Length:  3\r\n\r\na"
	// );
	// const std::string test1_expected_response =
	// 	"HTTP/1.1 200 OK\r\nConnection: close\r\nHost: sawa\r\n\r\nYou can't connect the dots "
	// 	"looking forward. You can only connect the dots looking backwards";
	// ret_code |= HandleResult(test1_response.CreateHttpResponse(1), test1_expected_response);

	// // リクエストのステータスコードが400の場合
	// http::Http test2_response;
	// test2_response.ParseHttpRequestFormat(
	// 	1, "GET / HTTP/1.\r\nHost test\r\nContent-Length:  3\r\n\r\na"
	// );
	// std::string test2_expected_response = LoadFileContent("expected_response/test2.txt");
	// ret_code |= HandleResult(test2_response.CreateHttpResponse(1), test2_expected_response);
	return ret_code;
}
