#include "stat.hpp"
#include "system_exception.hpp"
#include <cerrno>
#include <cstring>
#include <iostream>

namespace http {

Stat::Stat(const std::string &path) {
	if (stat(path.c_str(), &stat_buf_) == -1) {
		std::string error_message = "Error: stat on path '" + path + "': " + std::strerror(errno);
		throw utils::SystemException(error_message, errno);
	}
}

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
