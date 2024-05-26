#include <fstream>
#include <iostream>
#include <sstream>

bool FileExists(const std::string &file_path);

std::ifstream open_file(const std::string& file_path) {
    if (!FileExists(file_path)) {
        return std::ifstream("404.html");
    }
    return std::ifstream(file_path);
}

const std::string ReadFile(const std::string &file_path) {
    std::ifstream file = open_file(file_path);
	std::stringstream buffer;
	buffer << file.rdbuf();
	return buffer.str();
}

int main() {
	const std::string file_content = ReadFile("example.txt");
	std::cout << file_content << std::endl;
	const std::string not_file_exist = ReadFile("not_file_exist.txt");
	std::cout << not_file_exist << std::endl;
	const std::string not_authorized_file = ReadFile("not_authorized_file");
	std::cout << not_authorized_file << std::endl;
	return 0;
}
