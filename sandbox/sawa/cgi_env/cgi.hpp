#ifndef CGI_HPP_
#define CGI_HPP_

#include <map>
#include <string>

namespace cgi {
struct CGIRequest;

class CGI {
  public:
	typedef std::map<std::string, std::string> MetaMap;
	CGI();
	~CGI();
	int Run(const cgi::CGIRequest &request);

  private:
	CGI(const CGI &cgi);
	CGI   &operator=(const CGI &cgi);
	void   Set(const cgi::CGIRequest &request);
	char **SetCgiEnv();
	char **SetCgiArgv();
	void   Execve();
	void   ExecveCgiScript();
	void   Free();
	// cgi env;
	char      **env_;
	char      **argv_;
	MetaMap     meta_variables_;
	std::string body_message_;
	int         exit_status_;
	// R, W
	static const int R = 0;
	static const int W = 1;
};

} // namespace cgi

#endif
