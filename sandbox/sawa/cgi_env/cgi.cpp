#include "cgi.hpp"
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace cgi {

// CGIリクエストの構造体を渡す？
CGI::CGI() {}

// CGIリクエストの構造体の場合、Freeは削除する。んでデストラクターでfree
CGI::~CGI() {}

// CGIリクエストの情報を持つ構造体を引数に渡す
int CGI::Run(const char *script_name, const std::string &method) {
	Init(script_name, method);
	Execve();
	Free();
	return (this->exit_status_);
}

void CGI::Execve() {
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
		if (method == "POST") {
			close(cgi_request[W]);
			dup2(cgi_request[R], STDIN_FILENO);
			close(cgi_request[R]);
		}
		close(cgi_response[R]);
		dup2(cgi_response[W], STDOUT_FILENO);
		close(cgi_response[W]);
		ExecveCgiScript();
	} else {
		if (method == "POST") {
			// Send POST data to child process
			close(cgi_request[R]);
			// 標準入力でボディメッセージの情報を受け取る
			// std::string post_data = "name=ChatGPT&message=Hello";
			// std::cout << post_data.length() << std::endl;
			// write(cgi_request[W], post_data.c_str(), post_data.length());
			close(cgi_response[W]);
		}
		wait(NULL);
		char buf;
		close(cgi_response[W]);
		while (read(cgi_response[R], &buf, 1) > 0) {
			write(0, &buf, 1);
		}
		close(cgi_response[R]);
	}
}

// CGIのリクエスト情報をパースする
void CGI::Free() {
	for (size_t i = 0; this->argv_[i] != NULL; ++i) {
		delete[] this->argv_[i];
	}
	delete[] this->argv_;
	for (size_t i = 0; this->env_[i] != NULL; ++i) {
		delete[] this->env_[i];
	}
	delete[] this->env_;
}

void CGI::ExecveCgiScript() {
	// classでcgiの環境変数を保持
	this->exit_status_ = execve(cgi_script_, argv_, env_);
	// perror("execve"); // execveが失敗した場合のエラーメッセージ出力
}

// Initは必要ないかも、CGIクラスを呼ぶときに情報を渡す。
void CGI::Init(const char *script_name, const std::string &method) {
	this->argv_       = InitCgiArgv();
	this->env_        = InitCgiEnv();
	this->cgi_script_ = script_name;
	this->method_     = method;
}

char **CGI::InitCgiArgv() {
	// CGIスクリプトに引数を渡す場合の引数リストの初期化
	char **argv = new char *[2];
	argv[0]     = const_cast<char *>(cgi_script_);
	argv[1]     = NULL;
	return argv;
}

char **CGI::InitCgiEnv() {
	const std::map<std::string, std::string> &request_meta_variables =
		create_request_meta_variables();
	char **cgi_env = new char *[request_meta_variables.size() + 1];
	size_t i       = 0;
	for (std::map<std::string, std::string>::const_iterator ite = request_meta_variables.begin();
		 ite != request_meta_variables.end();
		 ite++) {
		const std::string element = ite->first + "=" + ite->second;
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

std::map<std::string, std::string> create_request_meta_variables() {
	std::map<std::string, std::string> request_meta_variables;
	request_meta_variables["AUTH_TYPE"];
	request_meta_variables["CONTENT_LENGTH"] = "26";
	request_meta_variables["CONTENT_TYPE"];
	request_meta_variables["GATEWAY_INTERFACE"] = "CGI/1.1";
	request_meta_variables["PATH_INFO"];
	request_meta_variables["PATH_TRANSLATED"];
	request_meta_variables["QUERY_STRING"];
	request_meta_variables["REMOTE_ADDR"];
	request_meta_variables["REMOTE_HOST"];
	request_meta_variables["REMOTE_IDENT"];
	request_meta_variables["REMOTE_USER"];
	request_meta_variables["REQUEST_METHOD"];
	request_meta_variables["SCRIPT_NAME"]     = "/cgi-bin/print_env.pl";
	request_meta_variables["SERVER_NAME"]     = "localhost";
	request_meta_variables["SERVER_PORT"]     = "8080";
	request_meta_variables["SERVER_PROTOCOL"] = "HTTP/1.1";
	request_meta_variables["SERVER_SOFTWARE"] = "Webserv/1.1";
	return request_meta_variables;
}

} // namespace cgi
