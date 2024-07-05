#ifndef CGI_HPP_
#define CGI_HPP_

#include <map>
#include <string>

namespace cgi {

std::map<std::string, std::string> create_request_meta_variables();

class CGI {
  public:
	CGI();
	~CGI();
	int Run(const char *script_name, const std::string &method);

  private:
	CGI(const CGI &cgi);
	CGI   &operator=(const CGI &cgi);
	void   Init(const char *script_name, const std::string &method);
	char **InitCgiEnv();
	char **InitCgiArgv();
	void   Execve();
	void   ExecveCgiScript();
	void   Free();
	// cgi env;
	char      **env_;
	char      **argv_;
	const char *cgi_script_;
	std::string method_;
	int         exit_status_;
	// R, W
	static const int R = 0;
	static const int W = 0;
};

} // namespace cgi

#endif
