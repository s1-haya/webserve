#ifndef CONFIG_PARSE_HPP_
#define CONFIG_PARSE_HPP_

#include "context.hpp"
#include "node.hpp"
#include <fstream>
#include <list>

#define ConfigInstance Config::GetInstance()

class Config {
  private:
	Config(const std::string &file_path);
	Config(const Config &);
	Config                &operator=(const Config &);
	static const Config   *s_cInstance;
	std::list<node::Node> *tokens_;
	std::ifstream          config_file_;

  public:
	~Config();

	static const Config          *GetInstance();
	static void                   Create(const std::string &file_path);
	static void                   Destroy();
	std::list<context::ServerCon> servers_;
};

#endif
