#include <cstring> // use strcpy
#include <iostream>
#include <map>

std::map<std::string, std::string> CreateRequestMetaVariables();

char **create_cgi_env(const std::map<std::string, std::string> &meta_variables) {
	char **cgi_env = new char *[meta_variables.size() + 1];
	size_t i = 0;
	typedef std::map<std::string, std::string>::const_iterator It;
	for (It it = meta_variables.begin(); it != meta_variables.end(); it++) {
		const std::string element = it->first + "=" + it->second;
		char             *dest    = new char[element.size() + 1];
		// todo error(new(std::nothrow))
		if (dest == NULL)
			return (NULL);
		std::strcpy(dest, element.c_str());
		cgi_env[i] = dest;
		i++;
	}
	cgi_env[i] = NULL;
	return cgi_env;
}
