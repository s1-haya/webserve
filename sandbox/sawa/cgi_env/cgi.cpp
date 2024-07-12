#include "cgi.hpp"
#include "cgi_parse.hpp"
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace cgi {

Cgi::Cgi() : argv_(NULL), env_(NULL), exit_status_(0) {}

// CGIリクエストの構造体の場合、Freeは削除する。んでデストラクターでfree
Cgi::~Cgi() {}

// CGIリクエストの情報を持つ構造体を引数に渡す。返り値enumを返す
int Cgi::Run(const cgi::CgiRequest &request) {
	SetCgiMember(request);
	Execve();
	Free();
	return (this->exit_status_);
}

void Cgi::Execve() {
	int cgi_request[2];
	int cgi_response[2];

	if (method_ == "POST" && pipe(cgi_request) == -1) {
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
		if (method_ == "POST") {
			close(cgi_request[WRITE]);
			dup2(cgi_request[READ], STDIN_FILENO);
			close(cgi_request[READ]);
		}
		close(cgi_response[READ]);
		dup2(cgi_response[WRITE], STDOUT_FILENO);
		close(cgi_response[WRITE]);
		ExecveCgiScript();
	} else {
		if (method_ == "POST") {
			close(cgi_request[READ]);
			write(cgi_request[WRITE], body_message_.c_str(), body_message_.length());
			close(cgi_request[WRITE]);
		}
		close(cgi_response[WRITE]);
		wait(NULL);
		char buf;
		while (read(cgi_response[READ], &buf, 1) > 0) {
			write(STDOUT_FILENO, &buf, 1);
		}
		close(cgi_response[READ]);
	}
}

void Cgi::Free() {
	for (size_t i = 0; this->argv_[i] != NULL; ++i) {
		delete[] this->argv_[i];
	}
	delete[] this->argv_;
	for (size_t i = 0; this->env_[i] != NULL; ++i) {
		delete[] this->env_[i];
	}
	delete[] this->env_;
}

void Cgi::ExecveCgiScript() {
	this->exit_status_ = execve(cgi_script_.c_str(), argv_, env_);
	// perror("execve"); // execveが失敗した場合のエラーメッセージ出力
}

void Cgi::SetCgiMember(cgi::CgiRequest request) {
	this->method_       = request.meta_variables["REQUEST_METHOD"];
	this->cgi_script_   = request.meta_variables["SCRIPT_NAME"];
	this->body_message_ = request.body_message;
	this->argv_         = SetCgiArgv();
	this->env_          = SetCgiEnv(request.meta_variables);
}

char *const* Cgi::SetCgiArgv() {
	char **argv = new char *[2];
	// todo error(new(std::nothrow))
	char             *dest    = new char[cgi_script_.size() + 1];
	// todo error(new(std::nothrow))
	std::strcpy(dest, cgi_script_.c_str());
	argv[0]     = dest;
	argv[1]     = NULL;
	return argv;
}

char *const* Cgi::SetCgiEnv(const MetaMap &meta_variables) {
	char                          **cgi_env = new char *[meta_variables.size() + 1];
	size_t                          i = 0;
	typedef MetaMap::const_iterator It;
	for (It it = meta_variables.begin(); it != meta_variables.end(); it++) {
		const std::string element = it->first + "=" + it->second;
		char             *dest    = new char[element.size() + 1];
		// todo error(new(std::nothrow))
		if (dest == NULL)
			return (NULL);
		std::strcpy(dest, element.c_str());
		cgi_env[i] = dest;
		i++;
	}
	cgi_env[i] = NULL;
	return cgi_env;
}

} // namespace cgi
