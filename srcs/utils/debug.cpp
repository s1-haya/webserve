#include "debug.hpp"

void Debug(const std::string &s) {
	std::cerr << COLOR_GLAY << s << COLOR_RESET << std::endl;
}

void Debug(const std::string &title, const std::string &s) {
	std::cerr << COLOR_GLAY << "[" << title << "] " << s << COLOR_RESET << std::endl;
}
