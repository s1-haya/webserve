#include "stat.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>

namespace {

void PrintStatStructMember(const struct stat &sb) {
	std::cout << "ID of containing device:  [" << major(sb.st_dev) << ", " << minor(sb.st_dev)
			  << "]" << std::endl;

	std::cout << "File type:                ";

	switch (sb.st_mode & S_IFMT) {
	case S_IFBLK:
		std::cout << "block device" << std::endl;
		break;
	case S_IFCHR:
		std::cout << "character device" << std::endl;
		break;
	case S_IFDIR:
		std::cout << "directory" << std::endl;
		break;
	case S_IFIFO:
		std::cout << "FIFO/pipe" << std::endl;
		break;
	case S_IFLNK:
		std::cout << "symlink" << std::endl;
		break;
	case S_IFREG:
		std::cout << "regular file" << std::endl;
		break;
	case S_IFSOCK:
		std::cout << "socket" << std::endl;
		break;
	default:
		std::cout << "unknown?" << std::endl;
		break;
	}

	std::cout << "I-node number:            " << (uintmax_t)sb.st_ino << std::endl;
	std::cout << "Mode:                     " << (uintmax_t)sb.st_mode << "(octal)" << std::endl;
	std::cout << "Link count:               " << (uintmax_t)sb.st_nlink << std::endl;
	std::cout << "Ownership:                UID=" << (uintmax_t)sb.st_uid
			  << ", GID=" << (uintmax_t)sb.st_gid << std::endl;
	std::cout << "Preferred I/O block size: " << (intmax_t)sb.st_blksize << "bytes" << std::endl;
	std::cout << "File size:                " << (intmax_t)sb.st_size << "bytes" << std::endl;
	std::cout << "Blocks allocated:         " << (intmax_t)sb.st_blocks << std::endl;
	std::cout << "Last status change:       " << ctime(&sb.st_ctime) << std::endl;
	std::cout << "Last file access:         " << ctime(&sb.st_atime) << std::endl;
	std::cout << "Last file modification:   " << ctime(&sb.st_mtime) << std::endl;
}

int GetTestCaseNum() {
	static int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

template <typename T>
int HandleResult(const T &result, const T &expected) {
	if (IsSame(result, expected)) {
		PrintOk();
		return EXIT_SUCCESS;
	} else {
		PrintNg();
		return EXIT_FAILURE;
	}
}
} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;

	try {
		utils::Stat        test("Makefile");
		const struct stat &makefile_info = test.GetStatBuffer();
		PrintStatStructMember(makefile_info);

		ret_code |= HandleResult(test.IsRegularFile(), true);
		ret_code |= HandleResult(test.IsReadableFile(), true);
		ret_code |= HandleResult(test.IsWritableFile(), true);
		ret_code |= HandleResult(test.IsDirectory(), false);
		std::cout << "Makefile size: " << test.GetFileSize() << std::endl;
	} catch (const utils::SystemException &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.GetErrorNumber() << std::endl;
	}

	try {
		utils::Stat test2("no.txt");
	} catch (const utils::SystemException &e) {
		std::cerr << e.what() << std::endl;
		std::cerr << e.GetErrorNumber() << std::endl;
	}
	return ret_code;
}
