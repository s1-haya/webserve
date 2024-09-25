#ifndef CGI_HPP_
#define CGI_HPP_

#include "cgi_request.hpp"
#include <map>
#include <string>

namespace cgi {

struct CgiResponse {
	// parse等の他の処理で決まるのでstatus_codeはここにはいらない
	std::string response;
	std::string content_type;
	bool        is_response_complete;
	CgiResponse(
		const std::string &response             = "",
		const std::string &content_type         = "",
		bool               is_response_complete = false
	)
		: response(response),
		  content_type(content_type),
		  is_response_complete(is_response_complete) {}
};

class Cgi {
  public:
	explicit Cgi(const CgiRequest &request);
	~Cgi();
	void Run();

	// pipe(),fork(),close()してpipe_fdをメンバにセット
	// pipe_fdのgetter
	int GetReadFd() const;
	int GetWriteFd() const;
	// read/writeのpipe_fdが存在するかどうか
	bool IsReadRequired() const;
	bool IsWriteRequired() const;
	// requestのgetter
	const std::string &GetRequest() const;
	// responseをaddしてget(全部送れたらresponse_completeをtrueにする)
	CgiResponse AddAndGetResponse(const std::string &read_buf);
	// write()が全部できなかった場合に送れなかった分だけ渡されるので差し替える
	void ReplaceNewRequest(const std::string &new_request_str);

	static const int READ;
	static const int WRITE;

  private:
	// prohibit copy constructor and assignment operator
	Cgi(const Cgi &cgi);
	Cgi &operator=(const Cgi &cgi);

	char *const *SetCgiEnv(const MetaMap &meta_variables);
	char *const *SetCgiArgv();
	void         Execve();
	void         ExecveCgiScript();
	void         Free();

	// cgi info;
	std::string  method_;
	std::string  cgi_script_;
	char *const *argv_;
	char *const *env_;
	int          exit_status_;
	std::string  request_body_message_;
	std::string  response_body_message_;

	pid_t pid_;

	int  read_fd_;
	int  write_fd_;
	bool is_response_complete_;
};

} // namespace cgi

#endif
