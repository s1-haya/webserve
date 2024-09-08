#ifndef HTTP_SERVERINFO_CHECK_HPP_
#define HTTP_SERVERINFO_CHECK_HPP_

#include "http_format.hpp"
#include "result.hpp"
#include "server_infos.hpp"
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
	std::string            upload_directory;

	utils::Result< std::pair<unsigned int, std::string> > redirect;
	utils::Result< std::pair<unsigned int, std::string> > error_page;
	CheckServerInfoResult() : autoindex(false) {
		redirect.Set(false);
		error_page.Set(false);
	};
};

class HttpServerInfoCheck {
  private:
	HttpServerInfoCheck();
	~HttpServerInfoCheck();

	/*====================================================*/ // 消す

	static void CheckDTOServerInfo(
		CheckServerInfoResult    &result,
		const MockDtoServerInfos &server_info,
		const HeaderFields       &header_fields
	);
	static void CheckLocationList(
		CheckServerInfoResult &result,
		const LocationList    &locations,
		const std::string     &request_target
	);
	static const MockLocationCon CheckLocation(
		CheckServerInfoResult &result,
		const LocationList    &locations,
		const std::string     &request_target
	);
	static void CheckIndex(CheckServerInfoResult &result, const MockLocationCon &location);
	static void CheckAutoIndex(CheckServerInfoResult &result, const MockLocationCon &location);
	static void CheckAlias(CheckServerInfoResult &result, const MockLocationCon &location);
	static void CheckRedirect(CheckServerInfoResult &result, const MockLocationCon &location);
	static void CheckAllowedMethods(CheckServerInfoResult &result, const MockLocationCon &location);
	static void CheckCgiExtension(CheckServerInfoResult &result, const MockLocationCon &location);
	static void
	CheckUploadDirectory(CheckServerInfoResult &result, const MockLocationCon &location);

	/*====================================================*/ // 消す

	static const server::VirtualServer *CheckVsList(
		const server::VirtualServerAddrList &virtual_servers, const HeaderFields &header_fields
	);
	static void CheckVirtualServer(
		CheckServerInfoResult       &result,
		const server::VirtualServer &virtual_server,
		const HeaderFields          &header_fields
	);

	typedef server::VirtualServer::LocationList VsLocationList;
	static void                                 CheckLocationList(
										CheckServerInfoResult &result,
										const VsLocationList  &locations,
										const std::string     &request_target
									);
	static const server::Location CheckLocation(
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
	static void
	CheckUploadDirectory(CheckServerInfoResult &result, const server::Location &location);

  public:
	static CheckServerInfoResult
	Check(const MockDtoServerInfos &server_info, const HttpRequestFormat &request);
	static CheckServerInfoResult
	Check(const server::VirtualServerAddrList &server_infos, const HttpRequestFormat &request);
};

} // namespace http

#endif
