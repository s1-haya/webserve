#ifndef CGI_HPP_
#define CGI_HPP_

namespace cgi {

class CGI {
	public:
		explicit CGI(const char *script_name);
		void Run();
		~CGI();
	
	private:
		CGI();
		CGI(const CGI &cgi);
		CGI &operator=(const CGI &cgi);
		void Init();
		char **InitCgiEnv();
		void ExecveCgiScript();
		// cgi env;
		char ** env_;
		const char *cgi_script_;
		// R, W
		static const int R = 0;
		static const int W = 0;

};

}

#endif
