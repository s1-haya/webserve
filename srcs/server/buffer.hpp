#ifndef SERVER_BUFFER_HPP_
#define SERVER_BUFFER_HPP_

#include "utils.hpp"
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

class Buffer {
  public:
	struct ReadBuf {
		ssize_t     read_size;
		std::string read_buf;
	};
	typedef utils::Result<ReadBuf> ReadResult;

	Buffer();
	~Buffer();
	// function
	ReadResult ReadRequest(int client_fd);

  private:
	// prohibit copy
	Buffer(const Buffer &other);
	Buffer &operator=(const Buffer &other);
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int BUFFER_SIZE  = 1024;
};

} // namespace server

#endif /* SERVER_BUFFER_HPP_ */
