#include "_config.hpp"
#include "utils.hpp"

namespace _config {

// todo: return or throw parse error
Config::ConfigData Config::ParseConfig(const std::string &path_config) {
	utils::Debug("config", "config path name => " + path_config);

	std::string tmp_config = "tmp_config_date";
	return tmp_config;
}

} // namespace _config
