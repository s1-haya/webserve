#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h>

bool fileExists(const std::string& filePath);

const std::string readFile(const std::string& filePath) {
    if (fileExists(filePath)) {
        std::ifstream file(filePath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return (buffer.str());
    } else {
        return ("404 Not Found");
    }
}

int main() {
    const std::string fileContent = readFile("example.txt");
    std::cout << fileContent << std::endl;
    const std::string notFileExist = readFile("notFileExist.txt");
    std::cout << notFileExist << std::endl;
    const std::string notAuthorizedFile = readFile("notAuthorizedFile");
    std::cout << notAuthorizedFile << std::endl;
    return 0;
}
