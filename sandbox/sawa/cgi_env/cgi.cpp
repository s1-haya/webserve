#include "cgi.hpp"
#include <iostream>
#include <map>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>

namespace cgi {

CGI::CGI(const char *script_name) : cgi_script_(script_name) {
	Init();
	std::cout << "Init: " << script_name << std::endl;
}

CGI::~CGI() {
	for (size_t i = 0; this->env_[i] != NULL; ++i) {
		delete[] this->env_[i];
	}
	delete[] this->env_;
}

void CGI::Run() {
	int cgi_request[2];
	int cgi_response[2];
	// class private: method
	std::string method = "POST";
	if (method == "POST" && pipe(cgi_request) == -1) {
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
		std::cout << "child" << std::endl;
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
			std::string post_data = "name=ChatGPT&message=Hello";
			std::cout << post_data.length() << std::endl;
			write(cgi_request[W], post_data.c_str(), post_data.length());
			close(cgi_response[W]);
		}
		wait(NULL);
		char buf;
		std::cout << "parent" << std::endl;
		close(cgi_response[W]);
		while (read(cgi_response[R], &buf, 1) > 0) {
			write(0, &buf, 1);
		}
		close(cgi_response[R]);
	}
}

void CGI::ExecveCgiScript(){
	// classでcgiの環境変数を保持
	char *const argv[] = {const_cast<char *>(cgi_script_), NULL};
	execve(cgi_script_, argv, env_);
	// perror("execve"); // execveが失敗した場合のエラーメッセージ出力
}

void CGI::Init() {
	this->env_ = InitCgiEnv();
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

char ** CGI::InitCgiEnv() {
	const std::map<std::string, std::string> &request_meta_variables =
		create_request_meta_variables();
	char **cgi_env = new char *[request_meta_variables.size() + 1];
	size_t       i       = 0;
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
} // namespace cgi
