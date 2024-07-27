#include "tmp_http.hpp"
#include "iostream"

namespace http {

Http::Http() {}

Http::~Http() {}

void Http::Run(int client_fd, const std::string& read_buf) {
	// todo: ClientSaveDataを取得してリクエストを読み込むかレスポンスを作成するかを判定する。(is_response)
	(void) client_fd;
	std::cout << read_buf << std::endl;
}

}

