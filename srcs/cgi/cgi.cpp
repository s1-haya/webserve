#include "cgi.hpp"
#include "cgi_request.hpp"
#include "http_message.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

namespace cgi {
namespace {

static const int SYSTEM_ERROR = -1;

int Close(int fd) {
	int status = close(fd);
	if (status == SYSTEM_ERROR) {
		throw SystemException(std::strerror(errno));
	}
	return status;
}

int Dup2(int fd1, int fd2) {
	int status = dup2(fd1, fd2);
	if (status == SYSTEM_ERROR) {
		throw SystemException(std::strerror(errno));
	}
	return status;
}

int Pipe(int fd[2]) {
	int status = pipe(fd);
	if (status == SYSTEM_ERROR) {
		throw SystemException(std::strerror(errno));
	}
	return status;
}

pid_t Fork(void) {
	pid_t p = fork();
	if (p == SYSTEM_ERROR) {
		throw SystemException(std::strerror(errno));
	}
	return p;
}

int Kill(pid_t pid, int sig) {
	int status = kill(pid, sig);
	if (status == SYSTEM_ERROR) {
		throw SystemException(std::strerror(errno));
	}
	return status;
}

pid_t Waitpid(pid_t pid, int *stat_loc, int options) {
	pid_t p = waitpid(pid, stat_loc, options);
	if (p == SYSTEM_ERROR) {
		throw SystemException(std::strerror(errno));
	}
	return p;
}

} // namespace

// 他のところでチェックしてここのatではthrowされない様にする
Cgi::Cgi(const CgiRequest &request)
	: method_(request.meta_variables.at(REQUEST_METHOD)),
	  cgi_script_(request.meta_variables.at(SCRIPT_NAME)),
	  argv_(SetCgiArgv()),
	  env_(SetCgiEnv(request.meta_variables)),
	  exit_status_(0),
	  request_body_message_(request.body_message),
	  pid_(-1),
	  read_fd_(-1),
	  write_fd_(-1),
	  is_response_complete_(false) {}

Cgi::~Cgi() {
	Free();
	if (read_fd_ != -1) {
		Close(read_fd_);
	}
	if (write_fd_ != -1) {
		Close(write_fd_);
	}
	if (pid_ > 0) {
		if (Waitpid(pid_, &exit_status_, WNOHANG) ==
			0) { // 子プロセスがまだ終了していない場合だけkillする
			Kill(pid_, SIGKILL);
			Waitpid(pid_, &exit_status_, 0);
		}
	}
}

void Cgi::Run() {
	try {
		Execve();
	} catch (const SystemException &e) {
		throw;
	}
}

void Cgi::Execve() {
	int cgi_request[2];
	int cgi_response[2];

	if (method_ == http::POST) {
		Pipe(cgi_request);
	}
	Pipe(cgi_response);
	pid_ = Fork();
	if (pid_ == 0) {
		if (method_ == http::POST) {
			Close(cgi_request[WRITE]);
			Dup2(cgi_request[READ], STDIN_FILENO);
			Close(cgi_request[READ]);
		}
		Close(cgi_response[READ]);
		Dup2(cgi_response[WRITE], STDOUT_FILENO);
		Close(cgi_response[WRITE]);
		ExecveCgiScript();
	}
	if (method_ == http::POST) {
		Close(cgi_request[READ]);
		write_fd_ = cgi_request[WRITE];
	}
	Close(cgi_response[WRITE]);
	read_fd_ = cgi_response[READ];
}

void Cgi::Free() {
	if (this->argv_ != NULL) {
		for (std::size_t i = 0; this->argv_[i] != NULL; ++i) {
			delete[] this->argv_[i];
		}
		delete[] this->argv_;
	}
	if (this->env_ != NULL) {
		for (std::size_t i = 0; this->env_[i] != NULL; ++i) {
			delete[] this->env_[i];
		}
		delete[] this->env_;
	}
}

void Cgi::ExecveCgiScript() {
	exit_status_ = execve(cgi_script_.c_str(), argv_, env_);
	std::exit(errno);
}

char *const *Cgi::SetCgiArgv() {
	char **argv = new (std::nothrow) char *[2];
	char  *dest = new (std::nothrow) char[cgi_script_.size() + 1];
	if (argv == NULL || dest == NULL) {
		throw SystemException("SetCgiArgv: Failed to allocate memory");
	}
	std::strcpy(dest, cgi_script_.c_str());
	argv[0] = dest;
	argv[1] = NULL;
	return argv;
}

char *const *Cgi::SetCgiEnv(const MetaMap &meta_variables) {
	char **cgi_env = new (std::nothrow) char *[meta_variables.size() + 1];
	if (cgi_env == NULL) {
		throw SystemException("SetCgiEnv: cgi_env: Failed to allocate memory");
	}
	std::size_t i = 0;

	typedef MetaMap::const_iterator It;
	for (It it = meta_variables.begin(); it != meta_variables.end(); ++it) {
		const std::string element = it->first + "=" + it->second;
		char             *dest    = new (std::nothrow) char[element.size() + 1];
		if (dest == NULL) {
			throw SystemException("SetCgiEnv: dest: Failed to allocate memory");
		}
		std::strcpy(dest, element.c_str());
		cgi_env[i] = dest;
		i++;
	}
	cgi_env[i] = NULL;
	return cgi_env;
}

int Cgi::GetReadFd() const {
	return read_fd_;
}

int Cgi::GetWriteFd() const {
	return write_fd_;
}

bool Cgi::IsReadRequired() const {
	return read_fd_ != -1;
}

bool Cgi::IsWriteRequired() const {
	return write_fd_ != -1;
}

const std::string &Cgi::GetRequest() const {
	return request_body_message_;
}

CgiResponse Cgi::AddAndGetResponse(const std::string &read_buf) {
	response_body_message_ += read_buf;
	if (read_buf.empty()) {
		is_response_complete_ = true;
	}
	return CgiResponse(response_body_message_, "text/plain", is_response_complete_);
	// text/plainのみ対応
}

void Cgi::ReplaceNewRequest(const std::string &new_request_str) {
	request_body_message_ = new_request_str;
}

} // namespace cgi
