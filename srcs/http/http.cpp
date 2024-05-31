#include "http.hpp"
#include "convert.hpp"

Http::Http(const std::string &read_buf) {
	(void)read_buf;
}

Http::~Http() {}

// todo: tmp response
std::string Http::CreateResponse() {
	const std::string content = "tmp";
	const std::string response =
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
		ToString(content.size()) + "\r\n\r\n" + content;
	return response;
}
