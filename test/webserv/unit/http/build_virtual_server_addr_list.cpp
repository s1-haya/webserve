/**
 * @brief VirtualServerの初期化
 * Locationのディレクティブを網羅するテストができるようなvirtual_serversを設定する。
 *
 * @param virtual_servers
 *
 * @return HttpResult indicating whether the response is complete
 *         and containing the response data.
 */

/** この設定を再現
server {
	# the port only
	listen 8080;
	server_name host host.com;

	# error_page
	error_page 404 /error_pages/404.html;

	# client_max_body_size (default 1m)
	client_max_body_size 2024;

	# alias
	# index
	location / {
		alias /html/;
		index index.html;
	}

	# upload_directory
	# allow_methods
	# autoindex
	location /upload {
		upload_dir /upload;
		allowed_methods GET POST DELETE;
		autoindex on;
	}

	# cgi_extension
	location /cgi-bin {
		cgi_extension .pl;
		allowed_methods GET POST;
	}

	# GET method not allowed
	location /get_not_allowed {
		allowed_methods DELETE;
	}
}

server {
	# the port and host
	listen 127.0.0.1:8000;
	server_name host2;

	# return
	location / {
		alias /html/;
		return 301 index.html;
	}
}
 */

#include "http_message.hpp"
#include "virtual_server.hpp"

namespace server {

typedef std::list<const VirtualServer *> VirtualServerAddrList;

}

// Location構造体を作成する関数
server::Location BuildLocation(
	const std::string                          &request_uri,
	const std::string                          &alias,
	const std::string                          &index,
	bool                                        autoindex,
	const std::list<std::string>               &allowed_methods,
	const std::pair<unsigned int, std::string> &redirect,
	const std::string                          &cgi_extension    = "",
	const std::string                          &upload_directory = ""
) {
	server::Location loc;
	loc.request_uri      = request_uri;
	loc.alias            = alias;
	loc.index            = index;
	loc.autoindex        = autoindex;
	loc.allowed_methods  = allowed_methods;
	loc.redirect         = redirect;
	loc.cgi_extension    = cgi_extension;
	loc.upload_directory = upload_directory;
	return loc;
}

// VirtualServer1を作成する関数
server::VirtualServer *BuildVirtualServer1() {
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host");
	server_names.push_back("host.com");

	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("0.0.0.0", 80));

	std::size_t client_max_body_size = 1024;

	server::VirtualServer::ErrorPage error_page(404, "/error_pages/404.html");

	server::VirtualServer::LocationList locationlist;
	// Location 1 - "/"
	std::list<std::string> allowed_methods_root;
	allowed_methods_root.push_back(http::GET);
	allowed_methods_root.push_back(http::POST);
	server::Location loc_root = BuildLocation(
		"/", "/html/", "index.html", false, allowed_methods_root, std::make_pair(0, "")
	);
	locationlist.push_back(loc_root);

	// Location 2 - "/upload"
	std::list<std::string> allowed_methods_save;
	allowed_methods_save.push_back(http::GET);
	allowed_methods_save.push_back(http::POST);
	allowed_methods_save.push_back(http::DELETE);
	server::Location loc_save = BuildLocation(
		"/upload/", "", "", true, allowed_methods_save, std::make_pair(0, ""), "", "/upload"
	);
	locationlist.push_back(loc_save);

	// Location 3 - "/cgi-bin"
	std::list<std::string> allowed_methods_cgi;
	allowed_methods_cgi.push_back(http::GET);
	allowed_methods_cgi.push_back(http::POST);
	server::Location loc_cgi = BuildLocation(
		"/cgi-bin/", "", "", false, allowed_methods_cgi, std::make_pair(0, ""), ".pl", ""
	);
	locationlist.push_back(loc_cgi);

	// Location 4 - "/get_not_allowed"
	std::list<std::string> get_not_allowed;
	get_not_allowed.push_back(http::DELETE);
	server::Location loc_get_not_allowed = BuildLocation(
		"/get_not_allowed/", "", "", false, get_not_allowed, std::make_pair(0, ""), ".pl", ""
	);
	locationlist.push_back(loc_get_not_allowed);

	return new server::VirtualServer(
		server_names, locationlist, host_ports, client_max_body_size, error_page
	);
}

// VirtualServer2を作成する関数
server::VirtualServer *BuildVirtualServer2() {
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host2");

	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("127.0.0.1", 8080));

	std::size_t client_max_body_size = 1024;

	server::VirtualServer::ErrorPage error_page;

	server::VirtualServer::LocationList locationlist;
	// Location 1 - "/"
	std::list<std::string> allowed_methods_root;
	server::Location       loc_root = BuildLocation(
        "/", "/html", "", false, allowed_methods_root, std::make_pair(301, "index.html")
    );
	locationlist.push_back(loc_root);

	return new server::VirtualServer(
		server_names, locationlist, host_ports, client_max_body_size, error_page
	);
}

// VirtualServerAddrListを作成する関数
server::VirtualServerAddrList BuildVirtualServerAddrList() {
	server::VirtualServerAddrList virtual_servers;
	virtual_servers.push_back(BuildVirtualServer1());
	virtual_servers.push_back(BuildVirtualServer2());
	return virtual_servers;
}

void DeleteVirtualServerAddrList(const server::VirtualServerAddrList &virtual_servers) {
	typedef server::VirtualServerAddrList::const_iterator ItVirtualServer;
	for (ItVirtualServer it = virtual_servers.begin(); it != virtual_servers.end(); ++it) {
		delete *it;
	}
}
