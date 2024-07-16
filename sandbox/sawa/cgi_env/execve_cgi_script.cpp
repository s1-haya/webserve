#include <iostream>
#include <map>
#include <unistd.h>

std::map<std::string, std::string> CreateRequestMetaVariables();
char **create_cgi_env(const std::map<std::string, std::string> &request_meta_variables);
char **create_cgi_argv(const char* cgi_script);

// void execve_cgi_script(const char *script_name) {
// 	const char *envp[] = {"CONTENT_LENGTH=26", NULL};
// 	// 未　execve error処理
// 	execve(script_name, NULL, envp);
// 	std::cerr << "Error: execve" << std::endl;
// }

void execve_cgi_script(const char *script_name) {
	// classでcgiの環境変数を保持
	char *const* envp = create_cgi_env(CreateRequestMetaVariables());
	char *const* argv = create_cgi_argv(script_name);
	execve(script_name, argv, const_cast<char *const *>(envp));
	// execveが失敗した場合のエラーメッセージ出力
}
