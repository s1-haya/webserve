#ifndef CGI_HPP_
#define CGI_HPP_

#include "cgi_request.hpp"
#include "utils.hpp"
#include <map>
#include <string>

namespace http {

class StatusCode;

} // namespace http

namespace cgi {

class Cgi {
  public:
	explicit Cgi(const CgiRequest &request);
	~Cgi();
	http::StatusCode Run(std::string &response_body_message);

	// <<< todo (関数名・変数名とか含め変えてしまって全然大丈夫です)
	typedef utils::Result<void> CgiResult;
	// pipe(),fork(),close()してpipe_fdをメンバにセット
	CgiResult Run();
	// pipe_fdのgetter
	int GetReadFd() const;
	int GetWriteFd() const;
	// read/writeのpipe_fdが存在するかどうか
	bool IsReadRequired() const;
	bool IsWriteRequired() const;
	// read()した結果をresponseに追加していく
	void AddReadBuf(const std::string &read_buf);
	// responseが完成したかどうかを取得
	bool IsResponseComplete() const;
	// responseのgetter(返り値がstruct CgiResponseになる？)
	const std::string &GetResponse() const;
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
