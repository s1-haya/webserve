#include "utils.hpp"
#include <cerrno>
#include <cstddef> // size_t
#include <cstdlib> // strtoul
#include <limits>  // numeric_limits
#include <sstream>

namespace utils {

Result<unsigned int> ConvertStrToUint(const std::string &str) {
	Result<unsigned int> convert_result(false, 0);
	if (str.empty() || !IsDigit(str[0])) {
		return convert_result;
	}

	char            *end;
	static const int BASE   = 10;
	errno                   = 0;
	const unsigned long num = std::strtoul(str.c_str(), &end, BASE);
	if (errno == ERANGE || *end != '\0' || num > std::numeric_limits<unsigned int>::max()) {
		return convert_result;
	}
	convert_result.Set(true, static_cast<unsigned int>(num));
	return convert_result;
}

std::string ConvertUintToStr(unsigned int num) {
	std::ostringstream oss;
	oss << num;
	return oss.str();
}

Result<std::size_t> ConvertStrToSize(const std::string &str) {
	Result<std::size_t> convert_result(false, 0);
	if (str.empty() || !IsDigit(str[0])) {
		return convert_result;
	}

	char            *end;
	static const int BASE   = 10;
	errno                   = 0;
	const unsigned long num = std::strtoul(str.c_str(), &end, BASE);
	if (errno == ERANGE || *end != '\0' || num > std::numeric_limits<size_t>::max()) {
		return convert_result;
	}
	convert_result.Set(true, static_cast<std::size_t>(num));
	return convert_result;
}

} // namespace utils
