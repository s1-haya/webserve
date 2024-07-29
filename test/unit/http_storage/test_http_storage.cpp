#include "http_storage.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

// httpクラスで使用するイメージ
// 1. ClientSaveDataがあるかどうか確認し、取得する。
// 	- ClientSaveDataがある場合はGetClientSaveDataから取得する
// 	- ClientSaveDataがない場合はCreateClientSaveDataを使ってclientのリクエスト情報を登録する
// 2. ClientSaveData.current_bufにhttpクラスからバッファサイズ分読み込んだ情報を追加する
// 3. ClientSaveData.IsHttpRequestFormatを使ってどこまでパースしたかを確認し、パースを試みる
// 	- 成功: IsHttpRequestFormat内の確認した書式のbool値をtrueにする。
// 	- 失敗: UpdateClientSaveDataでhttp_storageに保存し、処理を終了する。
// 4. 3.で成功した場合、is_responseを確認する
// 	- trueの場合: 処理を終了し、serverクラスはSendResponceにする
// 	- falseの場合: 3に戻る

template <typename T>
int HandleResult(const T &result, const T &expected) {
	if (result == expected) {
		std::cout << utils::color::GREEN << "[OK]" << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << "[NG] " << utils::color::RESET << std::endl;
		return EXIT_FAILURE;
	}
}

int main(void) {
	int               ret_code = 0;
	http::HttpStorage storage;

	// ClientSaveDataを新規作成
	http::ClientSaveData new_data = storage.GetClientSaveData(1);
	ret_code |= HandleResult(new_data.save_request_result.status_code, http::OK);

	// client_fdを登録したClientSaveData取得
	http::ClientSaveData save_data = storage.GetClientSaveData(1);
	ret_code |= HandleResult(save_data.save_request_result.status_code, http::OK);
	return ret_code;
}
