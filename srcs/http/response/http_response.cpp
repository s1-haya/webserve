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
	response_stream << response.status_line.version << SP << response.status_line.status_code << SP
					<< response.status_line.status_reason << CRLF;
	typedef HeaderFields::const_iterator It;
	for (It it = response.header_fields.begin(); it != response.header_fields.end(); it++) {
		response_stream << it->first << ":" << SP << it->second << CRLF;
	}
	response_stream << CRLF;
	response_stream << response.body_message;
	return response_stream.str();
}

} // namespace http
