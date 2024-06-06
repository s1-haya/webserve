#include "http.hpp"
#include <iostream>

int main(void) {
	Http http("test");
	std::cout << http.CreateResponse();
	return 0;
}
