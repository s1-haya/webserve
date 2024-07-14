#include <string>
#include <cstring>

char **create_cgi_argv(const char *cgi_script) {
	char **argv = new char *[2];
	// todo error(new(std::nothrow))
	char             *dest    = new char[strlen(cgi_script) + 1];
	// todo error(new(std::nothrow))
	std::strcpy(dest, cgi_script);
	argv[0]     = dest;
	argv[1]     = NULL;
	return argv;
}
