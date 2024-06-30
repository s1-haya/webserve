#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>

namespace config {

class Config {
  public:
	typedef std::string ConfigData;
	static ConfigData   ParseConfig(const std::string &path_config);

  private:
	// prohibit instantiation
	Config();
	~Config();
};

} // namespace config

#endif /* CONFIG_HPP_ */
