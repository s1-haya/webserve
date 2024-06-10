#ifndef CONFIG_PARSE_HPP_
#define CONFIG_PARSE_HPP_

#include "server.hpp"
#include <fstream>

#define ConfigInstance Config::GetInstance()

class Config {
  private:
	Config(const std::string &file_path);
	Config(const Config &);
	Config              &operator=(const Config &);
	static const Config *s_cInstance;

  public:
	~Config();

	static const Config *GetInstance();
	static void          Create(std::string &file_path);
	static void          Destroy();
	ServerDir            server_;
	std::ifstream        config_file_;
};

#endif
