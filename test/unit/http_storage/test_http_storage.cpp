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

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

int HandleResult(bool is_ok) {
	if (is_ok) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		return EXIT_FAILURE;
	}
}

int main(void) {
	int ret_code = 0;
	// ClientSaveDataが存在しない場合 -> OK
	ret_code |= HandleResult(!http::HttpStorage::IsClientSaveData(1));

	// ClientSaveDataが存在しない場合、ClientSaveDataを取得する -> Logic Error
	// http::ClientSaveData no_save_data = http::HttpStorage::GetClientSaveData(1);

	// ClientSaveDataが存在する場合 -> OK
	http::HttpStorage::CreateClientSaveData(1);
	ret_code |= HandleResult(http::HttpStorage::IsClientSaveData(1));

	// HttpStorageで保存したClientSaveDataを取得する -> OK
	http::ClientSaveData save_data = http::HttpStorage::GetClientSaveData(1);
	ret_code |= HandleResult(save_data.save_request_result.status_code == http::OK);

	// ClientSaveDataがすでに存在してる場合、ClientSaveDataを作成する -> Logic Error
	//  -> ClientSaveData already exists for client_fd 1
	// http::HttpStorage::CreateClientSaveData(1);

	// ClientDataを更新する(更新するデータはHTTPリクエストのステータスコード)-> OK
	http::ClientSaveData new_client_data;
	new_client_data.save_request_result.status_code = http::BAD_REQUEST;
	http::HttpStorage::UpdateClientSaveData(1, new_client_data);
	save_data = http::HttpStorage::GetClientSaveData(1);
	ret_code |= HandleResult(save_data.save_request_result.status_code == http::BAD_REQUEST);

	// 更新するClientDataが存在しない場合 -> Logic Error
	// http::HttpStorage::UpdateClientSaveData(2, new_client_data);

	// ClientSaveDataを削除する -> OK
	http::HttpStorage::DeleteClientSaveData(1);
	ret_code |= HandleResult(!http::HttpStorage::IsClientSaveData(1));

	// 削除するClientDataが存在しない場合 -> Logic Error
	// http::HttpStorage::DeleteClientSaveData(3);
	return ret_code;
}
