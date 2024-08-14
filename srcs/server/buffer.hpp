#ifndef SERVER_BUFFER_HPP_
#define SERVER_BUFFER_HPP_

#include <map>
#include <string>
#include <sys/types.h> // ssize_t

namespace server {

// todo: requestを保持してるのは仮。Httpに渡して任せる予定
class Buffer {
  public:
	// int: client_fd, string: request_buf
	typedef std::map<int, std::string> RequestMap;
	typedef std::map<int, std::string> ResponseMap;
	Buffer();
	~Buffer();
	// functions
	ssize_t ReadRequest(int client_fd);
	void    Delete(int client_fd);
	void    AddResponse(int client_fd, const std::string &response);
	// getter
	const std::string &GetRequest(int client_fd) const;
	const std::string &GetResponse(int client_fd) const;

  private:
	// prohibit copy
	Buffer(const Buffer &other);
	Buffer &operator=(const Buffer &other);
	// const
	static const int          SYSTEM_ERROR = -1;
	static const unsigned int BUFFER_SIZE  = 1024;
	// request buffers
	RequestMap  requests_;
	ResponseMap responses_;
};

} // namespace server

#endif /* SERVER_BUFFER_HPP_ */
