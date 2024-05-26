#include "Server.hpp"

// todo: set default config
Server::Server() : server_name_("default"), port_("8080") {}

// todo: set MapConfig -> private variables
Server::Server(Config::MapConfig &config)
	: server_name_(config["server_name"]), port_(config["port"]) {
	Debug("server", "set config");
}

Server::~Server() {}

void Server::Run() {
	Debug("server", "run server");
}
