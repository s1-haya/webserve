#include "http_storage.hpp"
#include <iostream>

namespace http {

HttpStorage::ClientSaveDataMap HttpStorage::save_data_;

// SaveDataを初期化する関数
void HttpStorage::CreateSaveData(int client_fd) {
	if (IsSaveData(client_fd)) {
		std::cerr << "SaveData already exists for client_fd " << client_fd << std::endl;
	} else {
		save_data_[client_fd] = SaveData();
	}
}

// SaveDataが存在するかを確認する関数
bool HttpStorage::IsSaveData(int client_fd) {
	return save_data_.find(client_fd) != save_data_.end();
}

// SaveDataを取得する関数
const SaveData &HttpStorage::GetSaveData(int client_fd) {
	try {
		return save_data_.at(client_fd);
	} catch (const std::logic_error& e) {
		throw std::logic_error("SaveData doesn't exists.");
	}
}

} // namespace http
