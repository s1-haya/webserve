#include "stat.hpp"
#include <iostream>

namespace utils {

Stat::Stat() {
	std::cout << "Called default constructor" << std::endl;
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

} // namespace utils
