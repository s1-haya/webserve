#ifndef SERVER_INFOS_HPP_
#define SERVER_INFOS_HPP_

#include <list>
#include <string>

namespace http {

// Mock(Serverの構造体に未実装の部分があるため)
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

struct MockLocationCon { /*一部serverで未実装*/
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect; // cannot use return
	std::string                          cgi_extension;
	std::string                          upload_directory;
	MockLocationCon() : autoindex(false) {}
};

typedef std::list<int>             PortList;
typedef std::list<MockLocationCon> LocationList;

struct MockDtoServerInfos {
	int                    server_fd;
	std::list<std::string> server_names;
	std::string            server_port;
	LocationList           locations;
	/*以下serverでは未実装*/
	std::string                          host;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
};

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

} // namespace http

#endif
