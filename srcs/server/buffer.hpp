#ifndef SERVER_BUFFER_HPP_
#define SERVER_BUFFER_HPP_

#include "utils.hpp"
#include <map>
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

// todo: requestを保持してるのは仮。Httpに渡して任せる予定
class Buffer {
  public:
	struct ReadBuf {
		ssize_t     read_size;
		std::string read_buf;
	};
	typedef utils::Result<ReadBuf> ReadResult;

	// int: client_fd, string: request_buf
	typedef std::map<int, std::string> RequestMap;
	Buffer();
	~Buffer();
	// functions
	ReadResult ReadRequest(int client_fd);
	void       Delete(int client_fd);
	// getter
	const std::string &GetRequest(int client_fd) const;

  private:
	// prohibit copy
	Buffer(const Buffer &other);
	Buffer &operator=(const Buffer &other);
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int BUFFER_SIZE  = 1024;
	// request buffers
	RequestMap requests_;
};

} // namespace server

#endif /* SERVER_BUFFER_HPP_ */
