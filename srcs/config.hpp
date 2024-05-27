#ifndef SERVER_CONFIG_HPP_
#define SERVER_CONFIG_HPP_

#include "debug.hpp" // todo: tmp
#include <string>

class Config {
  public:
	typedef std::string ConfigData;
	static ConfigData   ParseConfig(const std::string &path_config);

  private:
	// prohibit instantiation
	Config();
	~Config();
};

#endif /* SERVER_CONFIG_HPP_ */
