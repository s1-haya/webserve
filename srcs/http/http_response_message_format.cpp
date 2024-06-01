#include <iostream>
#include <map>
#include "character.hpp"

enum MessageType {
	HTTP_METHOD,
	HTTP_PATH,
	HTTP_CONTENT,
	HTTP_STATUS,
	HTTP_STATUS_TEXT,
};

typedef std::map<MessageType, std::string> RequestMessage;

void OutputStatusLine(RequestMessage request) {
	std::cout << "HTTP/1.1"
			  << SPACE << request[HTTP_STATUS] << SPACE << request[HTTP_STATUS_TEXT]
			  << std::endl;
}

int main(void) {
	RequestMessage request_;
	request_[HTTP_METHOD]  = "GET";
	request_[HTTP_PATH]    = "/html/index.html";
	request_[HTTP_STATUS]  = "200";
	request_[HTTP_STATUS_TEXT]  = "OK";
	request_[HTTP_CONTENT] = "<!DOCTYPE html><html<body><h1>Hello "
							 "from webserv!(tmp)</h1></body></html>";
	OutputStatusLine(request_);
	return 0;
}
