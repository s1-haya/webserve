#include "config.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <iostream>
#include <sstream>

const Config *Config::s_cInstance = NULL;

Config::Config(const std::string &file_path) : config_file_(file_path.c_str()) {
	if (!config_file_) {
		throw std::runtime_error("Cannot open Configuration file");
	}
	std::stringstream buffer;
	buffer << config_file_.rdbuf();
	tokens_ = new std::list<node::Node>;
	try {
		lexer::Lexer   lex(buffer.str(), *tokens_);
		parser::Parser par(*tokens_);
		servers_ = par.GetServers();
	} catch (const std::exception &e) {
		std::cerr << e.what() << '\n';
	}
	// try catchをどこでするか
}

Config::~Config() {
	if (config_file_)
		config_file_.close();
	if (tokens_)
		delete tokens_;
}

const Config *Config::GetInstance() {
	return s_cInstance;
}

void Config::Create(const std::string &file_path) {
	if (!s_cInstance)
		s_cInstance = new Config(file_path);
}

void Config::Destroy() {
	delete s_cInstance;
	s_cInstance = NULL;
}
