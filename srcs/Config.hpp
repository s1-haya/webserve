#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include "debug.hpp" // todo: tmp
#include <map>
#include <string>

class Config {
  public:
	// todo: tmp type
	typedef std::map<std::string, std::string> MapConfig;
	static MapConfig ParseConfig(const std::string &path_config);

  private:
	// prohibit instantiation
	Config();
	~Config();
};

#endif /* SERVER_CONFIG_HPP_ */
