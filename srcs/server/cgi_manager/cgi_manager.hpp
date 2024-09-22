#ifndef SERVER_CGI_MANAGER_HPP_
#define SERVER_CGI_MANAGER_HPP_

#include "cgi.hpp"
#include <map>

namespace server {

// "client_fd - cgi_instance", "pipe_fd - client_fd" を紐づけて全cgi_instanceを保持・取得
class CgiManager {
  public:
	typedef cgi::Cgi                   Cgi;
	typedef std::map<int, cgi::Cgi *>  CgiAddrMap;
	typedef CgiAddrMap::const_iterator ItrCgiAddrMap;
	typedef std::map<int, int>         FdMap;

	CgiManager();
	~CgiManager();

	// functions
	void AddNewCgi(int client_fd, const cgi::CgiRequest &request);

  private:
	// Prohibit copy
	CgiManager(const CgiManager &other);
	CgiManager &operator=(const CgiManager &other);

	// variables
	// client_fd毎にCgiをnewして保持
	CgiAddrMap cgi_addr_map_;
	// pipe_fdとclient_fdを紐づけ
	FdMap client_fd_map_;
};

} // namespace server

#endif /* SERVER_CGI_MANAGER_HPP_ */
