#ifndef SERVER_READ_HPP_
#define SERVER_READ_HPP_

#include "utils.hpp"
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

class Read {
  public:
	struct ReadBuf {
		ssize_t     read_size;
		std::string read_buf;
	};
	typedef utils::Result<ReadBuf> ReadResult;

	// function
	static ReadResult ReadStr(int client_fd);

  private:
	Read();
	~Read();
	// prohibit copy
	Read(const Read &other);
	Read &operator=(const Read &other);
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int BUFFER_SIZE  = 1024;
};

} // namespace server

#endif /* SERVER_READ_HPP_ */
