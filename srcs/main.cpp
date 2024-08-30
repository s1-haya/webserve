#include "color.hpp"
#include "config.hpp"
#include "server.hpp"
#include "utils.hpp"
#include <csignal>
#include <cstdlib> // EXIT_
#include <iostream>
#include <string>

namespace {

void PrintError(const std::string &s) {
	std::cerr << utils::color::RED << "Error: " << s << utils::color::RESET << std::endl;
}

static void PrintUsage() {
	std::cerr << "Usage:" << std::endl;
	std::cerr << "  ./webserv" << std::endl;
	std::cerr << "  ./webserv [configuration file]" << std::endl;
}

typedef utils::Result<void> SignalResult;

SignalResult SetSignalHandler() {
	SignalResult result;
	if (std::signal(SIGPIPE, SIG_IGN) == SIG_ERR) {
		result.Set(false);
	}
	return result;
}

} // namespace

int main(int argc, char **argv) {
	if (argc > 2) {
		PrintError("invalid arguments");
		PrintUsage();
		return EXIT_FAILURE;
	}
	const SignalResult result = SetSignalHandler();
	if (!result.IsOk()) {
		PrintError("signal failed");
		return EXIT_FAILURE;
	}

	std::string path_config;
	if (argc == 1) {
		path_config = "config/default.conf";
	} else {
		path_config = std::string(argv[1]);
	}
	try {
		config::ConfigInstance->Create(path_config);
		server::Server server(config::ConfigInstance->servers_);
		config::ConfigInstance->Destroy();
		server.Init();
		server.Run();
	} catch (const std::exception &e) {
		PrintError(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
