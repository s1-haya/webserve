#include "http_response.hpp"
#include "http_message.hpp"
#include <iostream>
#include <sstream>

namespace http {

// todo: リクエスト情報が正しいかどうかを確認し、HttpResponseResultを作成する関数（命名規則は仮）
// 引数はリクエスト情報とリクエストを送信したクライアントの仮想サーバーの情報
HttpResponseResult HttpResponse::CreateHttpResponseResult(const HttpRequestResult &request_info) {
	(void) request_info;
	// todo: okの場合　
	// todo: SelectLocation: リクエストターゲットがどの仮想サーバーのロケーションディレクトリに対応するか
		// 合致しない場合は 404のステータスコード
		// locationのpathが末尾にプレフィックスが付いてない場合。301のステータスコード
		// 例: ついてる場合 location /root/
			// ついてない場合 location /root
		// todo: root directiveの場合 root + request_target
			// - serverのrootよりlocationのrootが優先される
			// - rootが複数設定されてる場合は最初のroot以降は無視？
		// todo: CheckValidRequest:仮想サーバーのディレクトリからリクエストの有効性を確認する
		// - リソースがサーバー上に存在しない
		// - ファイルのアクセス権限ない
		// - メソッドが許可されてない
		// todo: cgiをリクエストしている場合は実行する。条件は以下だ。
		// - サーバーのディレクトリにcgi拡張子ファイルを許可するディレクティブがある場合
		// - リソースが上記のサーバーのディレクトリの場合
	// - メソッドがGET, POSTかつそれらが許可されてる場合
	// todo:　HttpRequestResultとServerConfig, LocationConfigを元にHttpResponseResultを作成する
	HttpResponseResult response;
	response.status_line.version         = HTTP_VERSION;
	response.status_line.status_code     = "200";
	response.status_line.status_reason   = "OK";
	response.header_fields["Host"]       = "sawa";
	response.header_fields["Connection"] = "close";
	response.body_message = "You can't connect the dots looking forword. You can only connect the "
							"dots looking backwards";
	return response;
}

// HttpResponseResult構造体をHttpResponseのフォーマットを文字列に出力する
std::string HttpResponse::CreateHttpResponse(const HttpResponseResult &response) {
	std::ostringstream response_stream;
	CreateStatusLine(response_stream, response.status_line);
	CreateHeaderFields(response_stream, response.header_fields);
	response_stream << CRLF;
	CreateBodyMessage(response_stream, response.body_message);
	return response_stream.str();
}

void HttpResponse::CreateStatusLine(std::ostream &response_stream, const StatusLine &status_line) {
	response_stream << status_line.version << SP << status_line.status_code << SP
					<< status_line.status_reason << CRLF;
}

void HttpResponse::CreateHeaderField(
	std::ostream &response_stream, const std::string &name, const std::string &value
) {
	response_stream << name << ":" << SP << value << CRLF;
}

void HttpResponse::CreateHeaderFields(
	std::ostream &response_stream, const HeaderFields &header_fields
) {
	typedef HeaderFields::const_iterator It;
	for (It it = header_fields.begin(); it != header_fields.end(); it++) {
		CreateHeaderField(response_stream, it->first, it->second);
	}
}

void HttpResponse::CreateBodyMessage(
	std::ostream &response_stream, const std::string &body_message
) {
	response_stream << body_message;
}

} // namespace http
