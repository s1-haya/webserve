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
	(void)client_fd;
	std::string tmp = "OK";
	return tmp;
}

} // namespace http
