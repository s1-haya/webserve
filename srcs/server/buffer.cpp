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
	responses_.erase(client_fd);
}

const std::string &Buffer::GetRequest(int client_fd) const {
	try {
		return requests_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("GetRequest() : client_fd doesn't exists");
	}
}

void Buffer::AddResponse(int client_fd, const std::string &response) {
	typedef std::pair<ResponseMap::const_iterator, bool> InsertResult;
	InsertResult result = responses_.insert(std::make_pair(client_fd, response));
	if (result.second == false) {
		throw std::logic_error("AddResponse() : client_fd is already added");
	}
}

const std::string &Buffer::GetResponse(int client_fd) const {
	try {
		return responses_.at(client_fd);
	} catch (const std::exception &e) {
		throw std::logic_error("GetResponse() : client_fd doesn't exists");
	}
}

} // namespace server
