#include <iostream>
#include <stdexcept>
#include <string>

void IsSame(bool condition, const char *file, int line) {
	if (!condition) {
		throw std::logic_error(std::string("Error at ") + file + ":" + std::to_string(line));
	}
}

#define IS_SAME(a, b) IsSame((a) == (b), __FILE__, __LINE__)

template <typename T>
void Compare(const T &a, const T &b) {
	IS_SAME(a, b);
}

int main() {
	try {
		int x = 5;
		int y = 10;
		Compare(x, y); // ここで例外がスローされる
	} catch (const std::logic_error &e) {
		std::cerr << "Logic error: " << e.what() << std::endl;
	}

	return 0;
}
