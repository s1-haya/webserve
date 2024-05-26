#include "config_parse.hpp"

#define PATH "src/index.html"

int main() {
	std::string conf = "config";
	ConfigInstance->Create(conf);
	std::cout << ConfigInstance->server_.server_name_ << std::endl;
}