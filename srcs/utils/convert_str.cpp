#include "utils.hpp"
#include <cerrno>
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

bool ConvertStrToSize(const std::string &str, size_t &num) {
	if (str.empty() || !IsDigit(str[0])) {
		return false;
	}

	char            *end;
	static const int BASE = 10;
	errno                 = 0;
	unsigned long tmp_num = std::strtoul(str.c_str(), &end, BASE);
	if (errno == ERANGE || *end != '\0' || tmp_num > std::numeric_limits<size_t>::max()) {
		return false;
	}
	num = static_cast<size_t>(tmp_num);
	return true;
}

} // namespace utils
