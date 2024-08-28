#include "tmp_http.hpp"
#include "http_message.hpp"
#include <iostream>

namespace http {

TmpHttp::TmpHttp() {}

TmpHttp::~TmpHttp() {}

// HttpResult Run(client_info, server_info) {
// 	// todo: ParseHttpRequestFormat
// 	// 	return HttpParsedRequestResult
// 	// - パースしたデータ
// 	// - 成功したかどうか
// 	ParseHttpRequestFormat(int client_fd, const std::string &read_buf);
// 	// todo: check is_parse
// 	if (!(HttpParsedRequestResult.is_success)) {
// 		HttpResult.result = CreateErrorHttpResponse(fd);
// 		HttpResult.is_complete = true;
// 		return HttpResult;
// 	}
// 	// private IsComplete;
// 	if (IsComplete(fd)) {
// 		HttpResult.result = CreateHttpResponse(fd);
// 		HttpResult.is_complete = true;
// 	}
// 	return HttpResult;
// }

// todo:
// HttpParsedRequestResult TmpHttp::ParseHttpRequestFormat(int client_fd, const std::string
// &read_buf) {
// HttpParsedRequestResult result;
// try {
// result = HttpParse::TmpRun(save_data);
// } catch () {
// result.is_success = false;
// }
// return HttpParsedRequestResult;
// }

// std::string CreateTimeoutRequest(client_fd) {
// 	HttpRequestParsedData data = storage_.GetClientSaveData(client_fd);
// 	storage_.DeleteClientSaveData(client_fd);
// 	return HttpResponse::CreateTimeoutRequest(data.request_result);
// }

// std::string CreateInternalServerError(client_fd)
// 	HttpRequestParsedData data = storage_.GetClientSaveData(client_fd);
// 	storage_.DeleteClientSaveData(client_fd);
// 	return HttpResponse::CreateInternalServerError(data.request_result);
// }

// todo: クライアントのリクエスト情報を読み込む
void TmpHttp::ParseHttpRequestFormat(int client_fd, const std::string &read_buf) {
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	save_data.current_buf += read_buf;
	HttpParse::TmpRun(save_data);
	storage_.UpdateClientSaveData(client_fd, save_data);
}

// todo: レスポンスを作成する
std::string TmpHttp::CreateHttpResponse(int client_fd) {
	HttpRequestParsedData data = storage_.GetClientSaveData(client_fd);
	storage_.DeleteClientSaveData(client_fd);
	return HttpResponse::Run(data.request_result);
}

// todo: パースで失敗した時
// std::string TmpHttp::CreateRequestParsedErrorHttpResponse(int client_fd) {
// 	HttpRequestParsedData data = storage_.GetClientSaveData(client_fd);
// 	storage_.DeleteClientSaveData(client_fd);
// ? Runではなくリクエストパースが失敗したようのレスポンスの方が良き？？
// 	return HttpResponse::Run(data.request_result);
// }

// todo: HTTPRequestの書式が完全かどうか(どのように取得するかは要検討)
bool TmpHttp::GetIsHttpRequestFormatComplete(int client_fd) {
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	return save_data.is_request_format.is_request_line &&
		   save_data.is_request_format.is_header_fields &&
		   save_data.is_request_format.is_body_message;
}

// For test
HttpRequestParsedData TmpHttp::GetClientData(int client_fd) {
	return storage_.GetClientSaveData(client_fd);
}

} // namespace http
