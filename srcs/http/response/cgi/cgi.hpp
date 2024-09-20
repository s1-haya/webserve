#ifndef CGI_HPP_
#define CGI_HPP_

#include <map>
#include <string>

namespace http {

class StatusCode;

namespace cgi {

struct CgiRequest;
typedef std::map<std::string, std::string> MetaMap;

class Cgi {
  public:
	explicit Cgi(const CgiRequest &request);
	~Cgi();
	StatusCode Run(std::string &response_body_message);

  private:
	// prohibit copy constructor and assignment operator
	Cgi(const Cgi &cgi);
	Cgi &operator=(const Cgi &cgi);

	char *const *SetCgiEnv(const MetaMap &meta_variables);
	char *const *SetCgiArgv();
	void         Execve();
	void         ExecveCgiScript();
	void         Free();

	// cgi info;
	std::string  method_;
	std::string  cgi_script_;
	char *const *argv_;
	char *const *env_;
	int          exit_status_;
	std::string  request_body_message_;
	std::string  response_body_message_;

	static const int READ  = 0;
	static const int WRITE = 1;
};

} // namespace cgi
} // namespace http

#endif
