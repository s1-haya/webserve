#ifndef SERVER_BUFFER_HPP_
#define SERVER_BUFFER_HPP_

#include "debug.hpp" // todo: tmp
#include <map>
#include <string>
#include <sys/types.h> // ssize_t

class Buffer {
  public:
	// int: fd, std::string: buffer
	typedef std::map<int, std::string> Buffers;
	Buffer();
	~Buffer();
	ssize_t     Read(int fd);
	void        Delete(int fd);
	std::string GetBuffer(int fd) const;

  private:
	// prohibit copy
	Buffer(const Buffer &other);
	Buffer &operator=(const Buffer &other);
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int BUFFER_SIZE  = 1024;
	// request buffers
	Buffers buffers_;
};

#endif /* SERVER_BUFFER_HPP_ */
