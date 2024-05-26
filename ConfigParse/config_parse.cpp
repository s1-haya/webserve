#include "config_parse.hpp"

ConfigParse::ConfigParse(std::string& file_path)
{
}

ConfigParse::~ConfigParse()
{
}

const ConfigParse* ConfigParse::GetInstance()
{
	return s_cInstance;
}

void ConfigParse::Create(std::string& file_path)
{
	if (!s_cInstance)
		s_cInstance = new ConfigParse(file_path);
}

void ConfigParse::Destroy()
{
	delete s_cInstance;
	s_cInstance = NULL;
}
