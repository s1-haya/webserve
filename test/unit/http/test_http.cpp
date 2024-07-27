#include "tmp_http.hpp"
#include <iostream>

int main(void) {
	http::TmpHttp test;
	test.RunParseRequest(1, "PARSE");
	const std::string &tmp_response = test.RunCreateResponse(1);
	std::cout << tmp_response;
	return EXIT_SUCCESS;
}
