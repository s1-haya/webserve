#include "config_parse.hpp"

const ConfigParse *ConfigParse::s_cInstance = NULL;

ConfigParse::ConfigParse(std::string &file_path) {
	server_.server_name_ = "localhost";
	std::ifstream     config_file(file_path);
	std::stringstream ss;
}

ConfigParse::~ConfigParse() {}

const ConfigParse *ConfigParse::GetInstance() {
	return s_cInstance;
}

void ConfigParse::Create(std::string &file_path) {
	if (!s_cInstance)
		s_cInstance = new ConfigParse(file_path);
}

void ConfigParse::Destroy() {
	delete s_cInstance;
	s_cInstance = NULL;
}
