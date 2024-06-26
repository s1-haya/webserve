#ifndef UTILS_CONVERT_HPP_
#define UTILS_CONVERT_HPP_

#include <sstream>
#include <string>

namespace utils {

template <typename T>
std::string ToString(T value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}

} // namespace utils

#endif /* UTILS_CONVERT_HPP_ */
