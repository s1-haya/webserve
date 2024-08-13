#ifndef CONFIG_PARSE_HPP_
#define CONFIG_PARSE_HPP_

#include "context.hpp"
#include "node.hpp"
#include <fstream>
#include <list>

#define ConfigInstance Config::GetInstance()

namespace config {

class Config {
  private:
	Config();
	explicit Config(const std::string &);
	Config(const Config &);
	Config              &operator=(const Config &);
	static const Config *s_cInstance;
	std::ifstream        config_file_;

  public:
	~Config();

	static const Config          *GetInstance();
	static void                   Create(const std::string &);
	static void                   Destroy();
	std::list<context::ServerCon> servers_;
};

} // namespace config

#endif
