#include "stat.hpp"
#include "utils.hpp"
#include <exception>
#include <iostream>

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

int main(void) {
	utils::Stat test;
	const struct stat &makefile_info = test.GetStatBuffer();
	PrintStatStructMember(makefile_info);
}
