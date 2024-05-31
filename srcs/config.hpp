#ifndef CONFIG_HPP_
#define CONFIG_HPP_

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

#endif /* CONFIG_HPP_ */
