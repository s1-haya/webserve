#include "config.hpp"

#define PATH "src/index.html"

int main() {
	std::string conf = "config";
	ConfigInstance->Create(conf);
	std::cout << ConfigInstance->server_.server_name_ << std::endl;
}