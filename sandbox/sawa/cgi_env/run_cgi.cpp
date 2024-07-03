#include <iostream>
#include <map>
#include <sys/wait.h>
#include <unistd.h>

#define R 0
#define W 1

void execve_cgi_script(const char *script_name);

void run_cgi(const char *scirpt_name) {
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
		execve_cgi_script(scirpt_name);
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
