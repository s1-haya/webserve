#include "parser.hpp"
#include "directive_names.hpp"
#include "result.hpp"
#include "utils.hpp"
#include <algorithm>
#include <stdexcept>

namespace config {
namespace parser {

Parser::Parser(std::list<node::Node> &tokens) : tokens_(tokens) {
	ParseNode();
}

Parser::~Parser() {}

void Parser::ParseNode() {
	for (NodeItr it(tokens_.begin(), tokens_.end()); it != tokens_.end(); ++it) {
		if ((*it).token_type == node::CONTEXT && (*it).token == SERVER) {
			servers_.push_back(CreateServerContext(++it));
		} else {
			throw std::runtime_error("expect server context");
		}
	}
}

namespace {

template <typename T>
bool FindDuplicated(const std::list<T> &list, const T &element) {
	if (std::find(list.begin(), list.end(), element) != list.end()) {
		return true;
	}
	return false;
}

bool IsDuplicateDirectiveName(
	std::set<std::string> &directive_set, const std::string &directive_name
) {
	if (directive_set.count(directive_name) == 0) {
		directive_set.insert(directive_name);
		return false;
	}
	return true;
}

} // namespace

/**
 * @brief Handlers for Server Context
 * @details Handlers for each Server Directive
 */

context::ServerCon Parser::CreateServerContext(NodeItr &it) {
	context::ServerCon server;

	server_directive_set_.clear();
	if ((*it).token_type != node::L_BRACKET) {
		throw std::runtime_error("expect { after server");
	}
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != node::R_BRACKET) {
		switch ((*it).token_type) {
		case node::DIRECTIVE:
			HandleServerContextDirective(server, it);
			break;
		case node::CONTEXT:
			if ((*it).token == LOCATION) {
				server.location_con.push_back(CreateLocationContext(++it));
			} else {
				throw std::runtime_error("invalid nest of 'server' directive");
			}
			break;
		case node::DELIM:
			throw std::runtime_error("unexpected ';'");
			break;
		default:
			throw std::runtime_error("unknown token");
			break;
		}
		++it;
	}
	if (it == tokens_.end()) {
		throw std::runtime_error("expect }");
	}
	return server;
}

void Parser::HandleServerContextDirective(context::ServerCon &server, NodeItr &it) {
	if ((*it).token == SERVER_NAME) {
		HandleServerName(server.server_names, ++it);
	} else if ((*it).token == LISTEN) {
		HandleListen(server.host_ports, ++it);
		// pair {host, port}
	} else if ((*it).token == CLIENT_MAX_BODY_SIZE) {
		HandleClientMaxBodySize(server.client_max_body_size, ++it);
	} else if ((*it).token == ERROR_PAGE) {
		HandleErrorPage(server.error_page, ++it);
	}
	if ((*it).token_type != node::DELIM) {
		throw std::runtime_error("expect ';' after arguments");
	}
}

void Parser::HandleServerName(std::list<std::string> &server_names, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'server_name' directive");
	}
	while ((*it).token_type != node::DELIM && (*it).token_type == node::WORD) {
		if (FindDuplicated(server_names, (*it).token)) {
			throw std::runtime_error("a duplicated parameter in 'server_name' directive");
		}
		server_names.push_back((*it++).token);
	} // server_name directive can be duplicated
}

void Parser::HandleListen(std::list<context::HostPortPair> &host_ports, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'listen' directive");
	}
	if (((*it).token).find(":") == std::string::npos) { // for listen 4242
		utils::Result<unsigned int> port_number = utils::ConvertStrToUint((*it).token);
		if (!port_number.IsOk() || port_number.GetValue() < PORT_MIN ||
			port_number.GetValue() > PORT_MAX) {
			throw std::runtime_error("invalid port number for ports");
		} else if (FindDuplicated(
					   host_ports, std::make_pair(std::string("0.0.0.0"), port_number.GetValue())
				   )) {
			throw std::runtime_error("a duplicated parameter in 'listen' directive");
		}
		host_ports.push_back(std::make_pair("0.0.0.0", port_number.GetValue()));
		++it;
		return;
	}
	// for listen localhost:8080
	std::vector<std::string> host_port_vec = utils::SplitStr((*it).token, ":");
	// 2個ないときは他の部分で例外が投げられる
	utils::Result<unsigned int> port_number = utils::ConvertStrToUint(host_port_vec[1]);
	if (host_port_vec[0] == "" || !port_number.IsOk() || port_number.GetValue() < PORT_MIN ||
		port_number.GetValue() > PORT_MAX) {
		throw std::runtime_error("invalid port number for ports");
	} else if (FindDuplicated(
				   host_ports, std::make_pair(host_port_vec[0], port_number.GetValue())
			   )) {
		throw std::runtime_error("a duplicated parameter in 'listen' directive");
	}
	// server_name directive can be duplicated
	host_ports.push_back(std::make_pair(host_port_vec[0], port_number.GetValue()));
	++it;
}

void Parser::HandleClientMaxBodySize(std::size_t &client_max_body_size, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'client_max_body_size' directive");
	}
	utils::Result<std::size_t> body_max_size = utils::ConvertStrToSize((*it).token);
	if (!body_max_size.IsOk()) { // check range?
		throw std::runtime_error("invalid client_max_body_size");
	}
	if (IsDuplicateDirectiveName(server_directive_set_, CLIENT_MAX_BODY_SIZE)) {
		throw std::runtime_error("'client_max_body_size' directive is duplicated");
	}
	client_max_body_size = body_max_size.GetValue();
	++it;
}

void Parser::HandleErrorPage(std::pair<unsigned int, std::string> &error_page, NodeItr &it) {
	if ((*it).token_type != node::WORD || (*++NodeItr(it)).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'error_page' directive");
	}
	// ex. 404 /404.html
	NodeItr tmp_it = it; // 404
	it++;                // /404.html
	utils::Result<unsigned int> status_code = utils::ConvertStrToUint((*tmp_it).token);
	if (!status_code.IsOk() || status_code.GetValue() < STATUS_CODE_MIN ||
		status_code.GetValue() > STATUS_CODE_MAX) {
		throw std::runtime_error("invalid status code for error_page");
	}
	if (IsDuplicateDirectiveName(server_directive_set_, ERROR_PAGE)) {
		throw std::runtime_error("'error_page' directive is duplicated");
	}
	error_page = std::make_pair(status_code.GetValue(), (*it).token);
	++it;
}

/**
 * @brief Handlers for Location Context
 * @details Handlers for each Location Directive
 */

context::LocationCon Parser::CreateLocationContext(NodeItr &it) {
	context::LocationCon location;

	location_directive_set_.clear();
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'location' directive");
	}
	location.request_uri = (*it).token;
	++it; // skip /www/
	if ((*it).token_type != node::L_BRACKET) {
		throw std::runtime_error("expect { after location argument");
	}
	++it; // skip L_BRACKET
	while (it != tokens_.end() && (*it).token_type != node::R_BRACKET) {
		switch ((*it).token_type) {
		case node::DIRECTIVE:
			HandleLocationContextDirective(location, it);
			break;
		case node::DELIM:
			throw std::runtime_error("unexpected ';'");
			break;
		case node::CONTEXT:
			throw std::runtime_error("invalid nest of 'location' directive"); // nginxではok
			break;
		default:
			throw std::runtime_error("unknown token");
			break;
		}
		++it;
	}
	if (it == tokens_.end()) {
		throw std::runtime_error("expect }");
	}
	return location;
}

void Parser::HandleLocationContextDirective(context::LocationCon &location, NodeItr &it) {
	if ((*it).token == ALIAS) {
		HandleAlias(location.alias, ++it);
	} else if ((*it).token == INDEX) {
		HandleIndex(location.index, ++it);
	} else if ((*it).token == AUTO_INDEX) {
		HandleAutoIndex(location.autoindex, ++it);
	} else if ((*it).token == ALLOWED_METHODS) {
		HandleAllowedMethods(location.allowed_methods, ++it);
	} else if ((*it).token == RETURN) {
		HandleReturn(location.redirect, ++it);
	} else if ((*it).token == CGI_EXTENSION) {
		HandleCgiExtension(location.cgi_extension, ++it);
	} else if ((*it).token == UPLOAD_DIR) {
		HandleUploadDirectory(location.upload_directory, ++it);
	}

	if ((*it).token_type != node::DELIM) {
		throw std::runtime_error("expect ';' after arguments");
	}
}

void Parser::HandleAlias(std::string &alias, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'alias' directive");
	}
	if (IsDuplicateDirectiveName(location_directive_set_, ALIAS)) {
		throw std::runtime_error("'alias' directive is duplicated");
	}
	alias = (*it++).token;
}

void Parser::HandleIndex(std::string &index, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'index' directive");
	}
	if (IsDuplicateDirectiveName(location_directive_set_, INDEX)) {
		throw std::runtime_error("'index' directive is duplicated");
	}
	index = (*it++).token;
}

void Parser::HandleAutoIndex(bool &autoindex, NodeItr &it) {
	if ((*it).token_type != node::WORD || ((*it).token != "on" && (*it).token != "off")) {
		throw std::runtime_error("invalid arguments in 'autoindex' directive");
	}
	if (IsDuplicateDirectiveName(location_directive_set_, AUTO_INDEX)) {
		throw std::runtime_error("'autoindex' directive is duplicated");
	}
	autoindex = ((*it++).token == "on");
}

void Parser::HandleAllowedMethods(std::list<std::string> &allowed_methods, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'allowed_methods' directive");
	}
	while ((*it).token_type != node::DELIM && (*it).token_type == node::WORD) {
		if (FindDuplicated(allowed_methods, (*it).token)) {
			throw std::runtime_error("a duplicated parameter in 'allowed_methods' directive");
		}
		allowed_methods.push_back((*it).token);
		if (std::find(
				VALID_ALLOWED_METHODS,
				VALID_ALLOWED_METHODS + SIZE_OF_VALID_ALLOWED_METHODS,
				(*it).token
			) == VALID_ALLOWED_METHODS + SIZE_OF_VALID_ALLOWED_METHODS) {
			throw std::runtime_error("an invalid method in 'allowed_methods' directive");
		}
		++it;
	}
	if (IsDuplicateDirectiveName(location_directive_set_, ALLOWED_METHODS)) {
		throw std::runtime_error("'allowed_methods' directive is duplicated");
	}
}

void Parser::HandleReturn(std::pair<unsigned int, std::string> &redirect, NodeItr &it) {
	if ((*it).token_type != node::WORD || (*++NodeItr(it)).token_type != node::WORD) {
		throw std::runtime_error("invalid number of arguments in 'return' directive");
	}
	// ex. 302 /index.html, tmp: atoi
	NodeItr tmp_it = it; // 302
	it++;                // /index.html
	utils::Result<unsigned int> status_code = utils::ConvertStrToUint((*tmp_it).token);
	if (!status_code.IsOk() || status_code.GetValue() < STATUS_CODE_MIN ||
		status_code.GetValue() > STATUS_CODE_MAX) {
		throw std::runtime_error("invalid status code for return");
	}
	if (IsDuplicateDirectiveName(location_directive_set_, RETURN)) {
		throw std::runtime_error("'return' directive is duplicated");
	}
	redirect = std::make_pair(status_code.GetValue(), (*it).token);
	++it;
}

void Parser::HandleCgiExtension(std::string &cgi_extension, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid arguments in 'cgi_extension' directive");
	}
	if (IsDuplicateDirectiveName(location_directive_set_, CGI_EXTENSION)) {
		throw std::runtime_error("'cgi_extension' directive is duplicated");
	}
	cgi_extension = (*it++).token;
}

void Parser::HandleUploadDirectory(std::string &upload_directory, NodeItr &it) {
	if ((*it).token_type != node::WORD) {
		throw std::runtime_error("invalid arguments in 'upload_directory' directive");
	}
	if (IsDuplicateDirectiveName(location_directive_set_, UPLOAD_DIR)) {
		throw std::runtime_error("'upload_directory' directive is duplicated");
	}
	upload_directory = (*it++).token;
}

std::list<context::ServerCon> Parser::GetServers() const {
	return this->servers_;
}

} // namespace parser
} // namespace config
