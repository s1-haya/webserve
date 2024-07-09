#include "http_parse.hpp"
#include <iostream>

int main(void) {
	http::HTTPParse   a;
	http::HTTPRequest request = a.Run("GET / HTTP1.1");
	std::cout << "Method: " << request.status_line.method << std::endl;
	std::cout << "uri: " << request.status_line.uri << std::endl;
	std::cout << "version: " << request.status_line.version << std::endl;
	return 0;
}
