#ifndef SOCK_CONTEXT_HPP_
#define SOCK_CONTEXT_HPP_

#include <map>

namespace server {

class SockInfo;
class ClientInfo;

// Store socket informations for each fd
class SockContext {
  public:
	typedef std::map<int, SockInfo>   SockInfoMap;
	typedef std::map<int, ClientInfo> ClientInfoMap;
	SockContext();
	~SockContext();
	// functions
	// todo: overload?
	void AddSockInfo(int fd, const SockInfo &sock_info);
	void DeleteSockInfo(int fd);
	void AddClientInfo(int fd, const ClientInfo &client_info);
	void DeleteClientInfo(int fd);
	// getter
	SockInfo &GetSockInfo(int fd);

  private:
	// prohibit copy
	SockContext(const SockContext &other);
	SockContext &operator=(const SockContext &other);
	// const
	static const int SYSTEM_ERROR = -1;
	// variables
	SockInfoMap   context_;
	ClientInfoMap client_context_;
};

} // namespace server

#endif /* SOCK_CONTEXT_HPP_ */
