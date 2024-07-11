#include "client.hpp"
#include "color.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <cstring> // strlen
#include <fstream> // ifstream
#include <iostream>
#include <sstream> // stringstream

namespace {

void PrintError(const std::string &s) {
	std::cerr << utils::color::RED << "Error: " << s << utils::color::RESET << std::endl;
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

// ./client PORT INFILE_PATH
int main(int argc, char **argv) {
	if (argc != 3) {
		PrintError("Error: invalid arguments");
		return EXIT_FAILURE;
	}

	try {
		const unsigned int port           = ConvertStrToUint(argv[1]);
		const std::string request_message = ReadFileStr(std::string(argv[2], std::strlen(argv[2])));
		Client            client(port);
		client.SendRequestAndReceiveResponse(request_message);
	} catch (const std::exception &e) {
		PrintError(e.what());
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
