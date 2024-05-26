#include <iostream>
#include <fstream>

bool fileExists(const std::string& filePath) {
    std::ifstream file(filePath.c_str());
    return (file.good());
}

int main() {
    const std::string& path = "./example.txt";
    if (fileExists(path)) {
        std::cout << "File exists." << std::endl;
    } else {
        std::cout << "File does not exist." << std::endl;
    }
    return (0);
}
