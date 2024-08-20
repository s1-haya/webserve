#include "buffer.hpp"
#include <cstdio>    // perror
#include <stdexcept> // logic_error
#include <unistd.h>  // read

namespace server {

Buffer::Buffer() {}

Buffer::~Buffer() {}

Buffer::ReadResult Buffer::ReadRequest(int client_fd) {
	ReadResult read_result;

	char    buffer[BUFFER_SIZE];
	ssize_t read_ret = read(client_fd, buffer, BUFFER_SIZE);
	if (read_ret <= 0) {
		if (read_ret == SYSTEM_ERROR) {
			perror("read failed");
			read_result.Set(false);
		}
		const ReadBuf read_buf = {read_ret, ""};
		read_result.SetValue(read_buf);
		return read_result;
	}
	requests_[client_fd] += std::string(buffer, read_ret);

	const ReadBuf read_buf = {read_ret, std::string(buffer, read_ret)};
	read_result.SetValue(read_buf);
	return read_result;
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
