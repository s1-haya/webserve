#ifndef SERVER_BUFFER_HPP_
#define SERVER_BUFFER_HPP_

#include <map>
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

class Buffer {
  public:
	// int: client_fd, string: request_buf
	typedef std::map<int, std::string> RequestMap;
	Buffer();
	~Buffer();
	ssize_t ReadRequest(int client_fd);
	void    DeleteRequest(int client_fd);
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
