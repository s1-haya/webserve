#include <iostream>
#include <map>
#include <unistd.h>

const char **create_cgi_env(const std::map<std::string, std::string> &request_meta_variables);

void execv_cgi_script(const char *script_name) {
	std::cout << script_name << std::endl;
	if (execve(script_name, NULL, NULL) == -1) {
		std::cerr << "Error executing script: " << strerror(errno) << std::endl;
	}
}
