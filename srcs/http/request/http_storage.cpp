#include "http_storage.hpp"
#include <iostream>

namespace http {

HttpStorage::ClientSaveDataMap HttpStorage::save_data_;

// ClientSaveDataを初期化する関数
void HttpStorage::CreateClientSaveData(int client_fd) {
	if (IsClientSaveData(client_fd)) {
		throw std::logic_error("ClientSaveData already exists.");
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
	try {
		return save_data_.at(client_fd);
	} catch (const std::logic_error &e) {
		throw std::logic_error("ClientSaveData doesn't exists.");
	}
}

// クライアント情報を更新する関数
void HttpStorage::UpdateClientSaveData(int client_fd, const ClientSaveData &client_data) {
	if (!IsClientSaveData(client_fd)) {
		throw std::logic_error("ClientSaveData doesn't exists.");
	} else {
		save_data_[client_fd] = client_data;
	}
}

// クライアント情報を削除する関数
void HttpStorage::DeleteClientSaveData(int client_fd) {
	if (save_data_.erase(client_fd) == 0) {
		throw std::logic_error("This save data of client doesn't exists.");
	}
}

} // namespace http
