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

int main(void) {
	try {
		if (http::HttpStorage::IsSaveData(1))
			http::SaveData save_data = http::HttpStorage::GetSaveData(1);
		else
			http::HttpStorage::CreateSaveData(1);
		http::SaveData save_data = http::HttpStorage::GetSaveData(1);
		std::cout << utils::color::GREEN << ".[OK]" << utils::color::RESET << std::endl;
	} catch (const std::exception &e) {
		std::cerr << utils::color::RED << ".[NG] " << utils::color::RESET << std::endl;
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return EXIT_SUCCESS;
}
