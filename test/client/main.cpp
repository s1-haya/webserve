#include "client.hpp"
#include <cstdlib>
#include <cstring> // strlen
#include <iostream>

namespace {
	static const unsigned int DEFAULT_PORT = 8080;

	static const std::string COLOR_RED   = "\033[31m";
	static const std::string COLOR_RESET = "\033[0m";

	void PrintError(const std::string &s) {
		std::cerr << COLOR_RED << "Error: " << s << COLOR_RESET << std::endl;
	}
} // namespace

// default message  : ./client
// original message : ./client "request message"
int main(int argc, char **argv) {
	try {
		Client client(DEFAULT_PORT);
		if (argc > 1) {
			const std::string message = std::string(argv[1], std::strlen(argv[1]));
			client.SetMessage(message);
		}
		client.SendRequestAndReceiveResponse();
	} catch (const std::exception &e) {
		PrintError(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
