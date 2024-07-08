#include "cgi.hpp"
#include "cgi_parse.hpp"
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace cgi {

CGI::CGI() {}

// CGIリクエストの構造体の場合、Freeは削除する。んでデストラクターでfree
CGI::~CGI() {}

// CGIリクエストの情報を持つ構造体を引数に渡す。返り値enumを返す
int CGI::Run(const cgi::CGIRequest &request) {
	Set(request);
	Execve();
	Free();
	return (this->exit_status_);
}

void CGI::Execve() {
	int cgi_request[2];
	int cgi_response[2];

	if (meta_variables_["REQUEST_METHOD"] == "POST" && pipe(cgi_request) == -1) {
		std::cerr << "Error: pipe" << std::endl;
		return;
	}
	if (pipe(cgi_response) == -1) {
		std::cerr << "Error: pipe" << std::endl;
		return;
	}
	pid_t p = fork();
	if (p < -1) {
		std::cerr << "Error: fork\n" << std::endl;
		return;
	} else if (p == 0) {
		// 親と子でプロセス空間が違うため、親プロセス自体の標準出力に影響はない。
		if (meta_variables_["REQUEST_METHOD"] == "POST") {
			close(cgi_request[W]);
			// close(STDIN_FILENO);
			dup2(cgi_request[R], STDIN_FILENO);
			close(cgi_request[R]);
		}
		close(cgi_response[R]);
		dup2(cgi_response[W], STDOUT_FILENO);
		close(cgi_response[W]);
		ExecveCgiScript();
	} else {
		if (meta_variables_["REQUEST_METHOD"] == "POST") {
			// Send POST data to child process
			close(cgi_request[R]);
			write(cgi_request[W], body_message_.c_str(), body_message_.length());
			close(cgi_request[W]);
		}
		close(cgi_response[W]);
		wait(NULL);
		char buf;
		while (read(cgi_response[R], &buf, 1) > 0) {
			write(STDOUT_FILENO, &buf, 1);
		}
		close(cgi_response[R]);
	}
}

// CGIのリクエスト情報をパースする
void CGI::Free() {
	// for (size_t i = 0; this->argv_[i] != NULL; ++i) {
	// 	delete[] this->argv_[i];
	// }
	delete[] this->argv_;
	for (size_t i = 0; this->env_[i] != NULL; ++i) {
		delete[] this->env_[i];
	}
	delete[] this->env_;
}

void CGI::ExecveCgiScript() {
	// classでcgiの環境変数を保持
	this->exit_status_ = execve(meta_variables_["SCRIPT_NAME"].c_str(), argv_, env_);
	// perror("execve"); // execveが失敗した場合のエラーメッセージ出力
}

// Setは必要ないかも、CGIクラスを呼ぶときに情報を渡す。
void CGI::Set(const cgi::CGIRequest &request) {
	this->meta_variables_ = request.meta_variables;
	this->body_message_   = request.body_message;
	this->argv_           = SetCgiArgv();
	this->env_            = SetCgiEnv();
}

char **CGI::SetCgiArgv() {
	// CGIスクリプトに引数を渡す場合の引数リストの初期化
	char **argv = new char *[2];
	argv[0]     = const_cast<char *>(meta_variables_["SCRIPT_NAME"].c_str());
	argv[1]     = NULL;
	return argv;
}

char **CGI::SetCgiEnv() {
	char                          **cgi_env = new char *[meta_variables_.size() + 1];
	typedef MetaMap::const_iterator It;
	size_t                          i = 0;
	for (It it = meta_variables_.begin(); it != meta_variables_.end(); it++) {
		const std::string element = it->first + "=" + it->second;
		cgi_env[i]                = new char[element.size() + 1];
		// error
		if (cgi_env[i] == NULL)
			return (NULL);
		std::strcpy(const_cast<char *>(cgi_env[i]), element.c_str());
		i++;
	}
	cgi_env[i] = NULL;
	return cgi_env;
}

} // namespace cgi
