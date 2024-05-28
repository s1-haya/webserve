#ifndef UTILS_DEBUG_HPP_
#define UTILS_DEBUG_HPP_

#include "color.hpp"
#include <iostream>
#include <string>

template <typename T>
void Debug(const T &x) {
	std::cerr << COLOR_GLAY << x << COLOR_RESET << std::endl;
}

template <typename T>
void Debug(const std::string &title, const T &x) {
	std::cerr << COLOR_GLAY << "[" << title << "] " << x << COLOR_RESET << std::endl;
}

#endif /* UTILS_DEBUG_HPP_ */
