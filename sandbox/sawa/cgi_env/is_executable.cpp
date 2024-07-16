#include <unistd.h>

bool is_executable(const char *file_path) {
	return access(file_path, X_OK) == 0;
}
