#include <iostream>
#include <list>
#include <string>

namespace {

#define ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))

template <typename T>
bool SearchListInArray(const std::list<T> &lst, const T *array, std::size_t array_size) {
	for (typename std::list<T>::const_iterator it = lst.begin(); it != lst.end(); ++it) {
		bool found = (std::find(array, array + array_size, *it) != (array + array_size));
		if (!found) {
			return false;
		}
	}
	return true;
}

} // namespace

int main() {
	std::string            array[] = {"GET", "POST"};
	std::list<std::string> list;
	list.push_back("GET");
	list.push_back("TEST");
	std::cout << std::boolalpha << SearchListInArray(list, array, ARRAY_SIZE(array)) << std::endl;
}
