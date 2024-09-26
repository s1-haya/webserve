#include "stat.hpp"
#include <iostream>

namespace http {

Stat::Stat(const struct stat &stat_buf) : stat_buf_(stat_buf) {}

Stat::~Stat() {}

Stat::Stat(const Stat &other) {
	*this = other;
}

Stat &Stat::operator=(const Stat &other) {
	if (this != &other) {
		this->stat_buf_ = other.stat_buf_;
	}
	return *this;
}

bool Stat::IsRegularFile() const {
	return (stat_buf_.st_mode & S_IFMT) == S_IFREG;
}

bool Stat::IsDirectory() const {
	return (stat_buf_.st_mode & S_IFMT) == S_IFDIR;
}

bool Stat::IsReadableFile() const {
	return IsRegularFile() && (stat_buf_.st_mode & S_IRUSR);
}

bool Stat::IsWritableFile() const {
	return IsRegularFile() && (stat_buf_.st_mode & S_IWUSR);
}

// for use max_body_size？？
std::size_t Stat::GetFileSize() const {
	if (IsDirectory()) {
		throw std::logic_error("This path is a directory.");
	} else {
		return stat_buf_.st_size;
	}
}

} // namespace http
