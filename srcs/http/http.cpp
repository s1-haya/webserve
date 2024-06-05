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
	std::string FileToString(const std::ifstream &file) {
		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	std::string ReadFile(const std::string &file_path) {
		std::ifstream file(file_path.c_str());
		if (!file) {
			std::ifstream error_file("html/404.html");
			return FileToString(error_file);
		}
		return FileToString(file);
	}
} // namespace

// todo: tmp content
void Http::ReadPathContent() {
	const std::string path = request_[HTTP_PATH];
	(void)path;
	(void)ReadFile("test");
	// todo: read path content
	const std::string content  = "<!DOCTYPE html><html<body><h1>Hello "
								 "from webserv!(tmp)</h1></body></html>";
	request_[HTTP_CONTENT]     = content;
	request_[HTTP_STATUS]      = "200";
	request_[HTTP_STATUS_TEXT] = "OK";
}
