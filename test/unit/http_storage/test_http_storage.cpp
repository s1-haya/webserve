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

int HandleResult(bool is_ok) {
	if (is_ok) {
		std::cout << utils::color::GREEN << "[OK]" << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << "[NG] " << utils::color::RESET << std::endl;
		return EXIT_FAILURE;
	}
}

int main(void) {
	// libc++abi: terminating due to uncaught exception of
	// type std::logic_error: ClientSaveData doesn't exists.
	// [1] 33298 abort./ a.out
	// ClientSaveDataが存在しない場合、ClientSaveDataを取得する -> Logic Error
	// http::ClientSaveData no_save_data = http::HttpStorage::GetClientSaveData(1);

	int ret_code = 0;
	// ClientSaveDataが存在しない場合 -> OK
	ret_code |= HandleResult(!http::HttpStorage::IsClientSaveData(1));
	// ClientSaveDataが存在する場合 -> OK
	http::HttpStorage::CreateClientSaveData(1);
	ret_code |= HandleResult(http::HttpStorage::IsClientSaveData(1));
	http::ClientSaveData save_data = http::HttpStorage::GetClientSaveData(1);
	// ClientSaveDataがすでに存在してる場合、ClientSaveDataを作成する
	//  -> ClientSaveData already exists for client_fd 1
	http::HttpStorage::CreateClientSaveData(1);
	return ret_code;
}
