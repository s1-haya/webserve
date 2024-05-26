#ifndef CONFIG_PARSE_HPP_
# define CONFIG_PARSE_HPP_

# include <iostream>
# include "server.hpp"

# define ConfigInstance ConfigParse::GetInstance()

class ConfigParse
{
private:
	ConfigParse(std::string& file_path);
	ConfigParse(const ConfigParse&);
	ConfigParse& operator=(const ConfigParse&);
	static const ConfigParse* s_cInstance;

public:
	~ConfigParse();

	static const ConfigParse* GetInstance();
	static void Create(std::string& file_path);
	static void Destroy();
};

#endif