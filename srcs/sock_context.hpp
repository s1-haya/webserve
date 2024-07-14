#ifndef SOCK_CONTEXT_HPP_
#define SOCK_CONTEXT_HPP_

#include <map>

namespace server {

class SockInfo;

// Store socket informations for each fd
class SockContext {
  public:
	typedef std::map<int, SockInfo> SockInfoMap;
	SockContext();
	~SockContext();
	// functions
	void AddSockInfo(int fd, const SockInfo &sock_info);
	void DeleteSockInfo(int fd);
	// getter
	SockInfo &GetSockInfo(int fd);

  private:
	// prohibit copy
	SockContext(const SockContext &other);
	SockContext &operator=(const SockContext &other);
	// const
	static const int SYSTEM_ERROR = -1;
	// variables
	SockInfoMap context_;
};

} // namespace server

#endif /* SOCK_CONTEXT_HPP_ */
