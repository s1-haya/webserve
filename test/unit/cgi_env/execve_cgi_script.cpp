#include <iostream>
#include <map>
#include <unistd.h>

const char **create_cgi_env(const std::map<std::string, std::string> &request_meta_variables);

void execve_cgi_script(const char *script_name) {
	// 未　execve error処理
	execve(script_name, NULL, NULL);
	std::cerr << "Error: execve" << std::endl;
}
