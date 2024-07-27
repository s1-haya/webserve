#include "tmp_http.hpp"
#include "iostream"

namespace http {

TmpHttp::TmpHttp() {}

TmpHttp::~TmpHttp() {}

// ServerクラスがClientSaveDataを取得してリクエストを読み込むかレスポンスを作成するかを判定する。(is_response)
// todo: クライアントのリクエスト情報を読み込む
void TmpHttp::RunParseRequest(int client_fd, const std::string &read_buf) {
	(void)client_fd;
	std::cout << read_buf << std::endl;
	// todo: リクエストを読むときはparse_Runする。返り値はClientSaveData
	// ClientSaveData save_data = parse_.Run(read_buf);
}

// todo: レスポンスを作成する
std::string TmpHttp::RunCreateResponse(int client_fd) {
	(void)client_fd;
	std::string tmp = "OK";
	return tmp;
}

} // namespace http
