#include "http.hpp"
#include "convert.hpp"
#include <fstream>
#include <sstream>

namespace http {

Http::Http(const std::string &read_buf) {
	ParseRequest(read_buf);
	ReadPathContent();
}

Http::~Http() {}

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
		utils::Debug("http", "404 file not found");
		return FileToString(error_file);
	}
	return FileToString(file);
}

} // namespace

// todo: tmp content
void Http::ReadPathContent() {
	const std::string content  = ReadFile(request_[HTTP_REQUEST_TARGET]);
	request_[HTTP_CONTENT]     = content;
	request_[HTTP_STATUS]      = "200";
	request_[HTTP_STATUS_TEXT] = "OK";
}

} // namespace http
