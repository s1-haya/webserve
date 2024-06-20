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

	unsigned int ConvertStrToUint(const std::string &str) {
		std::stringstream ss(str);
		int               num;
		ss >> num;
		if (num < 0 || !ss.eof() || ss.fail()) {
			throw std::logic_error("invalid port");
		}
		return static_cast<unsigned int>(num);
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
} // namespace

// default port & message  : ./client
// original port & message : ./client PORT INFILE_PATH
int main(int argc, char **argv) {
	if (argc != 1 && argc != 3) {
		PrintError("Error: invalid arguments");
		return EXIT_FAILURE;
	}

	try {
		unsigned int port;
		std::string  request_message;
		if (argc == 1) {
			port            = DEFAULT_PORT;
			request_message = ReadFileStr(DEFAULT_INFILE_PATH);
		} else {
			port = ConvertStrToUint(argv[1]);
			request_message =
				ReadFileStr(std::string(argv[2], std::strlen(argv[2])));
		}
		Client client(port, request_message);
		client.SendRequestAndReceiveResponse();
	} catch (const std::exception &e) {
		PrintError(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
