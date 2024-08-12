#include "stat.hpp"
#include <iostream>

namespace utils {

Stat::Stat(const std::string &path) {
	std::cout << "Called default constructor" << std::endl;
	// todo: error 処理行う
	stat(path.c_str(), &stat_buf_);
}

Stat::~Stat() {
	std::cout << "Called destructor" << std::endl;
}

Stat::Stat(const Stat &other) {
	(void)other;
	std::cout << "Called copy constructor" << std::endl;
}

Stat &Stat::operator=(const Stat &other) {
	(void)other;
	std::cout << "Called assignment operator" << std::endl;
	return *this;
}

const struct stat &Stat::GetStatBuffer() {
	return stat_buf_;
}

} // namespace utils
