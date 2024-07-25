#include "http_db.hpp"
#include <iostream>

http::HttpDb::HttpDb(int client_fd) {
	SaveData init_save_data;
	save_data_[client_fd] = init_save_data;
	std::cout << "Default Construct HttpDb!" << std::endl;
}
