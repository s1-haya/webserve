#include "buffer.hpp"
#include <stdio.h>  // perror
#include <unistd.h> // read

namespace server {

Buffer::Buffer() {}

Buffer::~Buffer() {}

ssize_t Buffer::Read(int fd) {
	char buffer[BUFFER_SIZE];

	ssize_t read_ret = read(fd, buffer, BUFFER_SIZE);
	if (read_ret <= 0) {
		if (read_ret == SYSTEM_ERROR) {
			perror("read failed");
		}
		return read_ret;
	}
	buffers_[fd] += std::string(buffer, read_ret);
	return read_ret;
}

void Buffer::Delete(int fd) {
	buffers_[fd].erase();
}

std::string Buffer::GetBuffer(int fd) const {
	// todo: fd error handle
	return buffers_.at(fd);
}

} // namespace server
