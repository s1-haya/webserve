#include "http_storage.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>

// httpクラスで使用するイメージ
// 1. SaveDataがあるかどうか確認し、取得する。
// 	- SaveDataがある場合はGetSaveDataから取得する
// 	- SaveDataがない場合はCreateSaveDataを使ってclientのリクエスト情報を登録する
// 2. SaveData.current_bufにhttpクラスからバッファサイズ分読み込んだ情報を追加する
// 3. SaveData.IsHttpRequestFormatを使ってどこまでパースしたかを確認し、パースを試みる
// 	- 成功: IsHttpRequestFormat内の確認した書式のbool値をtrueにする。
// 	- 失敗: UpdateSaveDataでhttp_storageに保存し、処理を終了する。
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
	// type std::logic_error: SaveData doesn't exists.
	// [1] 33298 abort./ a.out
	// SaveDataが存在しない場合、SaveDataを取得する -> Logic Error
	// http::SaveData no_save_data = http::HttpStorage::GetSaveData(1);	

	int ret_code = 0;
	// SaveDataが存在しない場合 -> OK
	ret_code |= HandleResult(!http::HttpStorage::IsSaveData(1));
	// SaveDataが存在する場合 -> OK
	http::HttpStorage::CreateSaveData(1);
	ret_code |= HandleResult(http::HttpStorage::IsSaveData(1));
	http::SaveData save_data = http::HttpStorage::GetSaveData(1);
	// SaveDataがすでに存在してる場合、SaveDataを作成する
	//  -> SaveData already exists for client_fd 1
	http::HttpStorage::CreateSaveData(1);
	return ret_code;
}
