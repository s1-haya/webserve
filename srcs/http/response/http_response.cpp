#include "http_response.hpp"
#include "http_message.hpp"
#include "http_parse.hpp"
#include "utils.hpp"
#include <iostream>
#include <sstream>

namespace http {

// todo:　HttpResponseParsedDataを元にHttpResponseResultを作成する
HttpResponseResult HttpResponse::CreateHttpResponseResult(const HttpRequestResult &request_info) {
	(void)request_info;
	HttpResponseResult response;
	response.status_line.version         = HTTP_VERSION;
	response.status_line.status_code     = "200";
	response.status_line.reason_phrase   = "OK";
	response.header_fields["Host"]       = "sawa";
	response.header_fields["Connection"] = "close";
	response.body_message = "You can't connect the dots looking forward. You can only connect the "
							"dots looking backwards";
	return response;
}

std::string HttpResponse::CreateDefaultBodyMessageFormat(
	const std::string &status_code, const std::string &reason_phrase
) {
	std::ostringstream body_message;
	body_message << "<html>" << CRLF << "<head><title>" << status_code << SP << reason_phrase
				 << "</title></head>" << CRLF << "<body>" << CRLF << "<center><h1>" << status_code
				 << SP << reason_phrase << "</h1></center>" << CRLF << "<hr><center>"
				 << SERVER_VERSION << "</center>" << CRLF << "</body>" << CRLF << "</html>" << CRLF;
	return body_message.str();
}

HttpResponseResult HttpResponse::CreateErrorHttpResponseResult(const HttpRequestResult &request_info
) {
	HttpResponseResult response;
	response.status_line.version       = HTTP_VERSION;
	response.status_line.status_code   = utils::ToString(request_info.status_code);
	response.status_line.reason_phrase = reason_phrase.at(request_info.status_code);
	// todo: StatusCodeをクラスにして、プライベートで保持する。
	// response.status_line.status_code   = request_info.status_code.GetStrStatusCode();
	// response.status_line.reason_phrase   = request_info.status_code.GetReasonPhrase();
	response.header_fields[CONTENT_TYPE] = "text/html";
	response.header_fields[SERVER]       = SERVER_VERSION;
	response.header_fields[CONNECTION]   = "close";
	response.body_message                = CreateDefaultBodyMessageFormat(
        response.status_line.status_code, response.status_line.reason_phrase
    );
	response.header_fields[CONTENT_LENGTH] = utils::ToString(response.body_message.length());
	return response;
}

// todo: CheckValidRequest:仮想サーバーとロケーションのディレクトリからリクエストの有効性を確認する
// HttpResponseParsedData HttpResponse::CheckValidRequest(const HttpRequestResult &request_info,
// const SeverConfig& server_config_info) {
// - リソースがサーバー上に存在しない
// - ファイルのアクセス権限ない
// - メソッドが許可されてない
// todo: cgiをリクエストしている場合は実行する。条件は以下だ。
// - サーバーのディレクトリにcgi拡張子ファイルを許可するディレクティブがある場合
// - リソースが上記のサーバーのディレクトリの場合
// - メソッドがGET, POSTかつそれらが許可されてる場合
// }

// HttpResponseResult構造体をHttpResponseのフォーマットを文字列に出力する
std::string HttpResponse::CreateHttpResponse(const HttpResponseResult &response) {
	std::ostringstream response_stream;
	response_stream << response.status_line.version << SP << response.status_line.status_code << SP
					<< response.status_line.reason_phrase << CRLF;
	typedef HeaderFields::const_iterator It;
	for (It it = response.header_fields.begin(); it != response.header_fields.end(); ++it) {
		response_stream << it->first << ":" << SP << it->second << CRLF;
	}
	response_stream << CRLF;
	response_stream << response.body_message;
	return response_stream.str();
}

} // namespace http
