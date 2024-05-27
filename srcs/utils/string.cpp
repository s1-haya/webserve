#include "string.hpp"
#include <sstream>

std::string ToString(int value) {
	std::ostringstream oss;
	oss << value;
	return oss.str();
}
