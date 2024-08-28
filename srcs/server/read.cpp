#include "read.hpp"
#include <cstdio>   // perror
#include <unistd.h> // read

namespace server {

Read::Read() {}

Read::~Read() {}

Read::ReadResult Read::ReadRequest(int client_fd) {
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
	const ReadBuf read_buf = {read_ret, std::string(buffer, read_ret)};
	read_result.SetValue(read_buf);
	return read_result;
}

} // namespace server
