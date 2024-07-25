#include "http_db.hpp"
#include <iostream>

namespace http {

std::map<int, SaveData> HttpDb::save_data_;

// SaveDataを初期化する関数
void HttpDb::CreateSaveData(int client_fd) {
	if (IsSaveData(client_fd)) {
		std::cerr << "SaveData already exists for client_fd " << client_fd << std::endl;
	} else {
		save_data_[client_fd] = SaveData();
	}
}
} // namespace http
