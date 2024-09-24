#ifndef CGI_HPP_
#define CGI_HPP_

#include "cgi_request.hpp"
#include "utils.hpp"
#include <map>
#include <string>

namespace cgi {

struct CgiResponse {
	// parse等の他の処理で決まるのでstatus_codeはここにはいらない
	std::string response;
	std::string content_type;
	CgiResponse(std::string response, std::string content_type)
		: response(response), content_type(content_type) {}
};

class Cgi {
	typedef utils::Result<CgiResponse> CgiResult;

  public:
	explicit Cgi(const CgiRequest &request);
	~Cgi();
	CgiResult Cgi::Run();

	// <<< todo (関数名・変数名とか含め変えてしまって全然大丈夫です)
	// pipe(),fork(),close()してpipe_fdをメンバにセット
	// pipe_fdのgetter
	int GetReadFd() const;
	int GetWriteFd() const;
	// read/writeのpipe_fdが存在するかどうか
	bool IsReadRequired() const;
	bool IsWriteRequired() const;
	// responseが完成したかどうかを取得
	bool IsResponseComplete() const;
	// requestのgetter
	const std::string &GetRequest() const;
	// responseをaddしてget(全部送れたらresponse_completeをtrueにする)
	const std::string &AddAndGetResponse(const std::string &read_buf);
	// write()が全部できなかった場合に送れなかった分だけ渡されるので差し替える
	void ReplaceNewRequest(const std::string &new_request_str);
	// >>> todo

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

	static const int READ  = 0;
	static const int WRITE = 1;

	pid_t pid_;

	// <<< todo
	// constructorの初期化も適当に-1にしてある
	int read_fd_;
	int write_fd_;
	// constructorの初期化をtrueにしてるけど本当はfalse。どこかでチェックしてフラグ変更する
	bool is_response_complete_;
	// >>> todo
};

} // namespace cgi

#endif
