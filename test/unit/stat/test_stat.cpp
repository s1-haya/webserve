#include "stat.hpp"

int main(void) {
	utils::Stat test;
	utils::Stat test2 = test;
	utils::Stat test3;
	test3 = test;
	return 0;
}
