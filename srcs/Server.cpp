#include "Server.hpp"

// todo: set default config
Server::Server() : server_name_("default"), port_(8080) {}

// todo: set tmp ConfigData -> private variables
Server::Server(const Config::ConfigData &config)
	: server_name_("from_config"), port_(8080) {
	Debug("server", "set config");
	(void)config;
}

Server::~Server() {}

void Server::Run() {
	Debug("server", "run server");
}
