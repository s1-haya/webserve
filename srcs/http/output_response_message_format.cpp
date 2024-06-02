#include "character.hpp"
#include <iostream>
#include <map>

enum MessageType {
	HTTP_METHOD,
	HTTP_PATH,
	HTTP_CONTENT,
	HTTP_STATUS,
	HTTP_STATUS_TEXT
};

typedef std::map<MessageType, std::string> RequestMessage;
void                                       OutputHeaderFields(void);

void OutputStatusLine(const RequestMessage &request) {
	std::cout << "HTTP/1.1" << SP << request.at(HTTP_STATUS) << SP
			  << request.at(HTTP_STATUS_TEXT) << CR << LF;
}

void OutputCRLF(void) {
	std::cout << CR << LF;
}

void OutputBody(const RequestMessage &request) {
	std::cout << request.at(HTTP_CONTENT) << std::endl;
}

void OutputResponse(const RequestMessage &request) {
	OutputStatusLine(request);
	OutputHeaderFields();
	OutputCRLF();
	OutputBody(request);
}
