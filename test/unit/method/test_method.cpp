#include "http_response.hpp"
#include <iostream>

int main(void) {
	std::string makefile;
	http::HttpResponse::GetHandler("Makefile", makefile);
	std::cout << makefile << std::endl;

	std::string not_found;
	http::HttpResponse::GetHandler("a", not_found);
	std::cout << not_found << std::endl;

	std::string redirect;
	http::HttpResponse::GetHandler("../../../test/unit", redirect);
	std::cout << redirect << std::endl;

	std::string forbidden;
	http::HttpResponse::GetHandler("forbidden_file", forbidden);
	std::cout << forbidden << std::endl;
}
