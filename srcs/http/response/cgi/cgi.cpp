#include "cgi.hpp"
#include "cgi_parse.hpp"
#include "http_exception.hpp"
#include "http_message.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

namespace http {
namespace cgi {
namespace {

static const int SYSTEM_ERROR = -1;

int Close(int fd) {
	int status = close(fd);
	if (status == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return status;
}

int Dup2(int fd1, int fd2) {
	int status = dup2(fd1, fd2);
	if (status == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return status;
}

int Pipe(int fd[2]) {
	int status = pipe(fd);
	if (status == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return status;
}

pid_t Fork(void) {
	pid_t p = fork();
	if (p == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return p;
}

ssize_t Write(int fd, const void *buf, size_t nbyte) {
	ssize_t bytes_write = write(fd, buf, nbyte);
	if (bytes_write == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return bytes_write;
}

ssize_t Read(int fd, void *buf, size_t nbyte) {
	ssize_t bytes_read = read(fd, buf, nbyte);
	if (bytes_read == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return bytes_read;
}

pid_t Waitpid(pid_t pid, int *stat_loc, int options) {
	pid_t p = waitpid(pid, stat_loc, options);
	if (p == SYSTEM_ERROR) {
		throw utils::SystemException(std::strerror(errno), errno);
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
	  read_fd_(-1),
	  write_fd_(-1) {}

Cgi::~Cgi() {
	Free();
}

StatusCode Cgi::Run(std::string &response_body_message) {
	try {
		Execve();
		response_body_message = response_body_message_;
	} catch (const utils::SystemException &e) {
		throw HttpException(e.what(), StatusCode(INTERNAL_SERVER_ERROR));
	}
	return StatusCode(OK);
}

void Cgi::Execve() {
	int cgi_request[2];
	int cgi_response[2];

	if (method_ == POST) {
		Pipe(cgi_request);
	}
	Pipe(cgi_response);
	pid_t p = Fork();
	if (p == 0) {
		if (method_ == POST) {
			Close(cgi_request[WRITE]);
			Dup2(cgi_request[READ], STDIN_FILENO);
			Close(cgi_request[READ]);
		}
		Close(cgi_response[READ]);
		Dup2(cgi_response[WRITE], STDOUT_FILENO);
		Close(cgi_response[WRITE]);
		ExecveCgiScript();
	}
	if (method_ == POST) {
		Close(cgi_request[READ]);
		write_fd_ = cgi_request[WRITE]; // todo: tmp
		Write(cgi_request[WRITE], request_body_message_.c_str(), request_body_message_.length());
		Close(cgi_request[WRITE]);
	}
	Close(cgi_response[WRITE]);
	read_fd_ = cgi_response[READ]; // todo: tmp
	char    buffer[1024];          // 読み取りバッファ
	ssize_t bytes_read;
	while ((bytes_read = Read(cgi_response[READ], buffer, sizeof(buffer))) > 0) {
		response_body_message_.append(buffer, bytes_read);
	}
	Close(cgi_response[READ]);
	Waitpid(p, &exit_status_, 0);
	if (WIFEXITED(exit_status_)) {
		if (WEXITSTATUS(exit_status_) != 0) {
			throw utils::SystemException("CGI script failed", WEXITSTATUS(exit_status_));
		}
	} else {
		throw utils::SystemException("CGI script did not exit normally", exit_status_);
		// exit_status_にはシグナル番号が入っている
	}
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
		throw utils::SystemException(std::strerror(errno), errno);
	}
	std::strcpy(dest, cgi_script_.c_str());
	argv[0] = dest;
	argv[1] = NULL;
	return argv;
}

char *const *Cgi::SetCgiEnv(const MetaMap &meta_variables) {
	char **cgi_env = new (std::nothrow) char *[meta_variables.size() + 1];
	if (cgi_env == NULL) {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	std::size_t i = 0;

	typedef MetaMap::const_iterator It;
	for (It it = meta_variables.begin(); it != meta_variables.end(); it++) {
		const std::string element = it->first + "=" + it->second;
		char             *dest    = new (std::nothrow) char[element.size() + 1];
		if (dest == NULL) {
			throw utils::SystemException(std::strerror(errno), errno);
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

} // namespace cgi
} // namespace http
