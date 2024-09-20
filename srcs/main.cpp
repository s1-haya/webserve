#include "config.hpp"
#include "server.hpp"
#include "start_up_exception.hpp"
#include "utils.hpp"
#include <csignal>
#include <cstdlib> // EXIT_
#include <iostream>
#include <string>

namespace {

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

// Throw server::StartUpException until server.Init() completes.
void RunServer() {
	while (true) {
		try {
			server::Server server(config::ConfigInstance->servers_);
			server.Init();
			server.Run();
		} catch (const server::StartUpException &e) {
			throw;
		} catch (const std::exception &e) {
			utils::PrintError(e.what());
		}
		utils::Debug("server", "re-run server");
	}
}

int main(int argc, char **argv) {
	if (argc > 2) {
		utils::PrintError("invalid arguments");
		PrintUsage();
		return EXIT_FAILURE;
	}
	const SignalResult result = SetSignalHandler();
	if (!result.IsOk()) {
		utils::PrintError("signal failed");
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
		RunServer();
		config::ConfigInstance->Destroy();
	} catch (const std::exception &e) {
		utils::PrintError(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
