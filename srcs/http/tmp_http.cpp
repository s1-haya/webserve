#include "tmp_http.hpp"
#include "iostream"

namespace http {

TmpHttp::TmpHttp() {}

TmpHttp::~TmpHttp() {}

// ServerクラスがClientSaveDataを取得してリクエストを読み込むかレスポンスを作成するかを判定する。(is_response)
// todo: クライアントのリクエスト情報を読み込む
void TmpHttp::ParseHttpRequestFormat(int client_fd, const std::string &read_buf) {
	std::cout << read_buf << std::endl;
	ClientSaveData save_data = storage_.GetClientSaveData(client_fd);
	save_data.save_current_buf += read_buf;
	// todo: HttpRequestParsedResult(HttpRequestResultとIsHttpRequestFormat)を受け取る。
	// save_data.result = http::HttpParse::parse_.Run(&save_current_buf);
storage_.UpdateClientSaveData(client_fd, save_data);
}

// todo: レスポンスを作成する
std::string TmpHttp::RunCreateResponse(int client_fd) {
// todo: HTTPRequestの書式が完全かどうか(どのように取得するかは要検討)
bool TmpHttp::GetIsHttpRequestFormatComplete(int client_fd) {
	ClientSaveData save_data = storage_.GetClientSaveData(client_fd);
	return save_data.save_is_request_format.is_request_line &&
		   save_data.save_is_request_format.is_header_fields &&
		   save_data.save_is_request_format.is_body_message;
}

} // namespace http
