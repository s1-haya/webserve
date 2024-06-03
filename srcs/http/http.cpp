#include "http.hpp"
#include "convert.hpp"
#include <fstream>
#include <sstream>

Http::Http(const std::string &read_buf) {
	ParseRequest(read_buf);
	ReadPathContent();
}

Http::~Http() {}

// todo: tmp request_
void Http::ParseRequest(const std::string &read_buf) {
	(void)read_buf;
	// todo: parse
	request_[HTTP_METHOD] = "GET";
	request_[HTTP_PATH]   = "/html/index.html";
}

namespace {
	bool FileExists(const std::string &file_path) {
		std::ifstream file(file_path.c_str());
		return file.good();
	}

	std::ifstream open_file(const std::string &file_path) {
		if (!FileExists(file_path)) {
			return std::ifstream("html/404.html");
		}
		return std::ifstream(file_path.c_str());
	}

	const std::string ReadFile(const std::string &file_path) {
		std::ifstream     file = open_file(file_path);
		std::stringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
} // namespace

// todo: tmp content
void Http::ReadPathContent() {
	const std::string content = ReadFile(request_[HTTP_PATH]);
	request_[HTTP_CONTENT]    = content;
	request_[HTTP_STATUS]     = "200";
}

// todo: tmp response
std::string Http::CreateResponse() {
	const std::string content = request_[HTTP_CONTENT];
	const std::string response =
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
		ToString(content.size()) + "\r\n\r\n" + content;
	return response;
}
