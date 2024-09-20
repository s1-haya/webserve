#ifndef CGI_MANAGER_HPP_
#define CGI_MANAGER_HPP_

#include "cgi.hpp" // todo: これもいらなくなるかも
#include <map>

namespace cgi {

using namespace http::cgi; // todo: remove(Cgiがhttpからcgiに移動したらこれ消せば良いはず)

// client_fdとcgi instance, pipe_fdとclient_fd を紐づけて全Cgiを保持・取得
class CgiManager {
  public:
	typedef std::map<int, Cgi *>       CgiAddrMap;
	typedef CgiAddrMap::const_iterator ItrCgiAddrMap;
	typedef std::map<int, int>         FdMap;

	CgiManager();
	~CgiManager();

	// functions
	void AddNewCgi(int client_fd, const CgiRequest &request);
	void DeleteCgi(int client_fd);
	void RunCgi(int client_fd);
	bool IsResponseComplete(int client_fd) const;
	void AddReadBuf(int client_fd, const std::string &read_buf);
	bool IsReadRequired(int client_fd) const;
	bool IsWriteRequired(int client_fd) const;
	// getter
	int                GetClientFd(int pipe_fd) const;
	const std::string &GetResponse(int client_fd) const;
	int                GetReadFd(int client_fd) const;
	int                GetWriteFd(int client_fd) const;

  private:
	// Prohibit copy
	CgiManager(const CgiManager &other);
	CgiManager &operator=(const CgiManager &other);
	// functions
	Cgi *GetCgi(int client_fd) const;
	bool IsClientFd(int fd) const;

	// client_fd毎にCgiAddrを保持
	CgiAddrMap client_cgi_map_;
	// pipe_fdとclient_fdを紐づけ
	FdMap pipe_fd_map_;
};

} // namespace cgi

#endif /* CGI_MANAGER_HPP_ */
