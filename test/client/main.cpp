#include "client.hpp"
#include <cstdlib>
#include <cstring> // strlen
#include <fstream> // ifstream
#include <iostream>
#include <sstream> // stringstream

namespace {
	static const unsigned int DEFAULT_PORT = 8080;
	static const std::string  DEFAULT_INFILE_PATH =
		"request_messages/200_get_root.txt";

	static const std::string COLOR_RED   = "\033[31m";
	static const std::string COLOR_RESET = "\033[0m";

	void PrintError(const std::string &s) {
		std::cerr << COLOR_RED << "Error: " << s << COLOR_RESET << std::endl;
	}

	std::string FileToString(const std::ifstream &file) {
		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}

	std::string ReadFileStr(const std::string &file_path) {
		std::ifstream file(file_path.c_str());
		if (!file) {
			throw std::runtime_error("error infile");
		}
		return FileToString(file);
	}

	std::string GetRequestMessage(int argc, char **argv) {
		if (argc <= 1) {
			return ReadFileStr(DEFAULT_INFILE_PATH);
		}
		return ReadFileStr(std::string(argv[1], std::strlen(argv[1])));
	}
} // namespace

// default message  : ./client
// original message : ./client INFILE_PATH
int main(int argc, char **argv) {
	try {
		const std::string request_message = GetRequestMessage(argc, argv);
		Client            client(DEFAULT_PORT, request_message);
		client.SendRequestAndReceiveResponse();
	} catch (const std::exception &e) {
		PrintError(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
