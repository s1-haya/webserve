#include "utils.hpp"
#include <cerrno>
#include <cstdlib> // strtoul
#include <limits>  // numeric_limits
#include <sstream>

namespace utils {

bool ConvertStrToUint(const std::string &str, unsigned int &num) {
	if (str.empty() || !IsDigit(str[0])) {
		return false;
	}

	char            *end;
	static const int BASE = 10;
	errno                 = 0;
	unsigned long tmp_num = std::strtoul(str.c_str(), &end, BASE);
	if (errno == ERANGE || *end != '\0' || tmp_num > std::numeric_limits<unsigned int>::max()) {
		return false;
	}
	num = static_cast<unsigned int>(tmp_num);
	return true;
}

std::string ConvertUintToStr(unsigned int num) {
	std::ostringstream oss;
	oss << num;
	return oss.str();
}

} // namespace utils
