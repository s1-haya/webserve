#include "config.hpp"
#include <sstream>

const Config *Config::s_cInstance = NULL;

Config::Config(std::string &file_path) : config_file_(file_path) {
	if (!config_file_)
		std::cerr << "Cannot open Configuration file" << std::endl;
	server_.server_name_ = "localhost";
	std::stringstream buffer;
	buffer << config_file_.rdbuf();
	std::cout << buffer.str() << std::endl;
}

Config::~Config() {
	if (config_file_)
		config_file_.close();
}

const Config *Config::GetInstance() {
	return s_cInstance;
}

void Config::Create(std::string &file_path) {
	if (!s_cInstance)
		s_cInstance = new Config(file_path);
}

void Config::Destroy() {
	delete s_cInstance;
	s_cInstance = NULL;
}
