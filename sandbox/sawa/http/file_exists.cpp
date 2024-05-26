#include <iostream>
#include <fstream>

bool FileExists(const std::string& file_path) {
    std::ifstream file(file_path.c_str());
    return (file.good());
}

// int main() {
//     const std::string& path = "./example.txt";
//     if (fileExists(path)) {
//         std::cout << "File exists." << std::endl;
//     } else {
//         std::cout << "File does not exist." << std::endl;
//     }
//     const std::string& notAuthorizedFilePath = "notAuthorizedFile";
//     if (fileExists(notAuthorizedFilePath)) {
//         std::cout << "File exists." << std::endl;
//     } else {
//         std::cout << "File does not exist." << std::endl;
//     }
//     return (0);
// }
