#include <iostream>
#include <map>
#include <unistd.h>

std::map<std::string, std::string> create_request_meta_variables();
const char **create_cgi_env(const std::map<std::string, std::string> &request_meta_variables);

// void execve_cgi_script(const char *script_name) {
// 	const char *envp[] = {"CONTENT_LENGTH=26", NULL};
// 	// 未　execve error処理
// 	execve(script_name, NULL, envp);
// 	std::cerr << "Error: execve" << std::endl;
// }

void execve_cgi_script(const char *script_name) {
	// classでcgiの環境変数を保持
	char *const argv[] = {const_cast<char*>(script_name), NULL};
	const char **envp = create_cgi_env(create_request_meta_variables());
	execve(script_name, argv, const_cast<char *const *>(envp));
}
