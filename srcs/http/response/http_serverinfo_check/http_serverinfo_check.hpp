#ifndef HTTP_SERVERINFO_CHECK_HPP_
#define HTTP_SERVERINFO_CHECK_HPP_

#include "http_format.hpp"
#include "result.hpp"
#include "virtual_server.hpp"
#include <list>
#include <string>

namespace server {

typedef std::list<const VirtualServer *> VirtualServerAddrList;

}

namespace http {

struct CheckServerInfoResult {
	std::string path; // alias, indexを見る
	std::string index;
	bool        autoindex;

	// メソッドの処理で使用
	std::list<std::string> allowed_methods;
	std::string            cgi_extension;
	std::string            file_upload_path;

	utils::Result< std::pair<unsigned int, std::string> > redirect;
	utils::Result< std::pair<unsigned int, std::string> > error_page;

	std::string host_name;
	std::string server_port;
	CheckServerInfoResult() : autoindex(false) {
		redirect.Set(false);
		error_page.Set(false);
	};
};

class HttpServerInfoCheck {
  private:
	HttpServerInfoCheck();
	~HttpServerInfoCheck();

	static const server::VirtualServer *FindVirtualServer(
		const server::VirtualServerAddrList &virtual_servers, const HeaderFields &header_fields
	);
	static void CheckVirtualServer(
		CheckServerInfoResult       &result,
		const server::VirtualServer &virtual_server,
		const HeaderFields          &header_fields,
		std::size_t                  request_body_size
	);

	typedef server::VirtualServer::LocationList VsLocationList;
	static void                                 CheckLocationList(
										CheckServerInfoResult &result,
										const VsLocationList  &locations,
										const std::string     &request_target
									);
	static const server::Location FindLocation(
		CheckServerInfoResult &result,
		const VsLocationList  &locations,
		const std::string     &request_target
	);
	static void CheckIndex(CheckServerInfoResult &result, const server::Location &location);
	static void CheckAutoIndex(CheckServerInfoResult &result, const server::Location &location);
	static void CheckAlias(CheckServerInfoResult &result, const server::Location &location);
	static void CheckRedirect(CheckServerInfoResult &result, const server::Location &location);
	static void
	CheckAllowedMethods(CheckServerInfoResult &result, const server::Location &location);
	static void CheckCgiExtension(CheckServerInfoResult &result, const server::Location &location);
	static void CheckUploadPath(CheckServerInfoResult &result, const server::Location &location);

  public:
	static CheckServerInfoResult
	Check(const server::VirtualServerAddrList &server_infos, const HttpRequestFormat &request);
};

} // namespace http

#endif
