#include "buffer.hpp"
#include <cstdio>    // perror
#include <stdexcept> // logic_error
#include <unistd.h>  // read

namespace server {

Buffer::Buffer() {}

Buffer::~Buffer() {}

ssize_t Buffer::ReadRequest(int client_fd) {
	char buffer[BUFFER_SIZE];

	ssize_t read_ret = read(client_fd, buffer, BUFFER_SIZE);
	if (read_ret <= 0) {
		if (read_ret == SYSTEM_ERROR) {
			perror("read failed");
		}
		return read_ret;
	}
	requests_[client_fd] += std::string(buffer, read_ret);
	return read_ret;
}

void Buffer::Delete(int client_fd) {
	requests_.erase(client_fd);
}

const std::string &Buffer::GetRequest(int client_fd) const {
	try {
		return requests_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("GetRequest() : client_fd doesn't exists");
	}
}

} // namespace server
