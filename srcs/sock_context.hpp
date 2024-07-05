#ifndef SOCK_CONTEXT_HPP_
#define SOCK_CONTEXT_HPP_

#include <map>

namespace server {

class SockInfo;

// Store socket informations for each fd
class SockContext {
  public:
	typedef std::map<int, SockInfo> SockInfos;

  private:
	SockContext();
	~SockContext();
	// prohibit copy
	SockContext(const SockContext &other);
	SockContext &operator=(const SockContext &other);
	// const
	static const int SYSTEM_ERROR = -1;
	// variables
	SockInfos context_;
};

} // namespace server

#endif /* SOCK_CONTEXT_HPP_ */
