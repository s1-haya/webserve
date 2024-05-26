#include <iostream>
#include <fstream>
#include <sstream>

bool FileExists(const std::string& file_path);

const std::string ReadFile(const std::string& file_path) {
    if (FileExists(file_path)) {
        std::ifstream file(file_path);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    } else {
        return "404 Not Found";
    }
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
