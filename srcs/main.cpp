#include "Config.hpp"
#include "Server.hpp"
#include <cstdlib> // EXIT_
#include <iostream>
#include <string>

// ./webserv [configuration file]
int main(int argc, char **argv) {
	if (argc > 2) {
		std::cerr << "Error: invalid arguments" << std::endl;
		return EXIT_FAILURE;
	}

	// todo: handle parse error
	std::string path_config;
	if (argc == 1) {
		path_config = "default.conf";
	} else {
		path_config = std::string(argv[1]);
	}
	Config::MapConfig config = Config::ParseConfig(path_config);

	// todo: try-catch?
	Server server(config);
	server.Run();

	return EXIT_SUCCESS;
}
