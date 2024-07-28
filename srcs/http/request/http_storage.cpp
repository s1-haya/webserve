#include "http_storage.hpp"
#include <iostream>

namespace http {

HttpStorage::ClientSaveDataMap HttpStorage::save_data_;

// ClientSaveDataを初期化する関数
void HttpStorage::CreateClientSaveData(int client_fd) {
	if (IsClientSaveData(client_fd)) {
		std::cerr << "ClientSaveData already exists for client_fd " << client_fd << std::endl;
	} else {
		save_data_[client_fd] = ClientSaveData();
	}
}

// ClientSaveDataが存在するかを確認する関数
bool HttpStorage::IsClientSaveData(int client_fd) {
	return save_data_.find(client_fd) != save_data_.end();
}

// ClientSaveDataを取得する関数
const ClientSaveData &HttpStorage::GetClientSaveData(int client_fd) {
	if (!IsClientSaveData(client_fd)) {
		CreateClientSaveData(client_fd);
	}
	return save_data_.at(client_fd);
}

} // namespace http
