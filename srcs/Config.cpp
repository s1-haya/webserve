#include "Config.hpp"

// todo: return or throw parse error
Config::MapConfig Config::ParseConfig(const std::string &path_config) {
	Debug("config", "config path name => " + path_config);

	// todo: tmp settings
	Config::MapConfig config;
	config["server_name"] = "kari";
	config["port"]        = "8080";
	return config;
}
