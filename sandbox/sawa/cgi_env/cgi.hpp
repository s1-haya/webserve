#ifndef CGI_HPP_
#define CGI_HPP_

#include <map>
#include <string>

namespace cgi {

struct CgiRequest;

class Cgi {
  public:
	typedef std::map<std::string, std::string> MetaMap;
	Cgi();
	~Cgi();
	int Run(const cgi::CgiRequest &request);

  private:
	Cgi(const Cgi &cgi);
	Cgi   &operator=(const Cgi &cgi);
	void   SetCgiMember(cgi::CgiRequest request);
	char *const* SetCgiEnv(const MetaMap &meta_variables);
	char *const* SetCgiArgv();
	void   Execve();
	void   ExecveCgiScript();
	void   Free();
	// cgi info;
	std::string method_;
	std::string cgi_script_;
	std::string body_message_;
	char      *const* argv_;
	char      *const* env_;
	int         exit_status_;

	static const int READ = 0;
	static const int WRITE = 1;
};

} // namespace cgi

#endif
