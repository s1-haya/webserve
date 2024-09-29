#ifndef SERVER_CGI_MANAGER_HPP_
#define SERVER_CGI_MANAGER_HPP_

#include "cgi.hpp"
#include "utils.hpp"
#include <map>

namespace server {

// "client_fd - cgi_instance", "pipe_fd - client_fd" を紐づけて全cgi_instanceを保持・取得
class CgiManager {
  public:
	typedef cgi::Cgi                   Cgi;
	typedef std::map<int, cgi::Cgi *>  CgiAddrMap;
	typedef CgiAddrMap::const_iterator ItrCgiAddrMap;
	typedef std::map<int, int>         FdMap;
	typedef utils::Result<int>         GetFdResult;

	CgiManager();
	~CgiManager();

	// functions
	void               AddNewCgi(int client_fd, const cgi::CgiRequest &request);
	void               RunCgi(int client_fd);
	void               DeleteCgi(int client_fd);
	GetFdResult        GetReadFd(int client_fd) const;
	GetFdResult        GetWriteFd(int client_fd) const;
	int                GetClientFd(int pipe_fd) const;
	const std::string &GetRequest(int client_fd) const;
	cgi::CgiResponse   AddAndGetResponse(int client_fd, const std::string &read_buf);
	void               ReplaceNewRequest(int client_fd, const std::string &new_request_str);
	bool               IsCgiExist(int fd) const;

  private:
	// Prohibit copy
	CgiManager(const CgiManager &other);
	CgiManager &operator=(const CgiManager &other);

	// functions
	Cgi       *GetCgi(int client_fd);
	const Cgi *GetCgi(int client_fd) const;

	// variables
	// client_fd毎にCgiをnewして保持
	CgiAddrMap cgi_addr_map_;
	// pipe_fdとclient_fdを紐づけ
	FdMap client_fd_map_;
};

} // namespace server

#endif /* SERVER_CGI_MANAGER_HPP_ */
