#include <iostream>
#include <map>
#include <sys/wait.h>
#include <unistd.h>

#define R 0
#define W 1

void execve_cgi_script(const char *script_name);

void run_cgi(const char *scirpt_name) {
	int pipefd[2];

	if (pipe(pipefd) == -1) {
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
		close(pipefd[R]);
		dup2(pipefd[W], STDOUT_FILENO);
		close(pipefd[W]);
		execve_cgi_script(scirpt_name);
	} else {
		close(pipefd[W]);
		wait(NULL);
		std::cout << "parent" << std::endl;
		char buf;
		while (read(pipefd[R], &buf, 1) > 0) {
			write(0, &buf, 1);
		}
		close(pipefd[R]);
	}
}
