#ifndef UTILS_HPP_
#define UTILS_HPP_

#include "color.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace utils {

template <typename T>
void Debug(const T &x) {
	std::cerr << color::GRAY << x << color::RESET << std::endl;
}

template <typename T>
void Debug(const std::string &title, const T &x) {
	std::cerr << color::GRAY << "[" << title << "] " << x << color::RESET << std::endl;
}

template <typename T, typename U>
void Debug(const std::string &title, const T &x, const U &y) {
	std::cerr << color::GRAY << "[" << title << "] " << x << "(" << y << ")" << color::RESET
			  << std::endl;
}

template <typename T>
std::string ToString(T value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

// string
bool                     ConvertStrToUint(const std::string &str, unsigned int &num);
bool                     ConvertStrToSize(const std::string &str, size_t &num);
std::string              ConvertUintToStr(unsigned int num);
std::vector<std::string> SplitStr(const std::string &src, const std::string &substring);

bool IsDigit(char c);

} // namespace utils

#endif /* UTILS_HPP_ */
