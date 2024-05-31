#include "http.hpp"
#include "convert.hpp"

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

// todo: tmp content
void Http::ReadPathContent() {
	const std::string path = request_[HTTP_PATH];
	(void)path;
	// todo: read path content
	const std::string content = "<!DOCTYPE html><html<body><h1>Hello "
								"from webserv!(tmp)</h1></body></html>";
	request_[HTTP_CONTENT]    = content;
	request_[HTTP_STATUS]     = "200";
}

// todo: tmp response
std::string Http::CreateResponse() {
	const std::string content = "tmp";
	const std::string response =
		"HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " +
		ToString(content.size()) + "\r\n\r\n" + content;
	return response;
}
