#ifndef HTTP_SERVERINFO_CHECK_HPP_
#define HTTP_SERVERINFO_CHECK_HPP_

#include "http_format.hpp"
#include "result.hpp"
#include "server_infos.hpp"
#include <list>
#include <string>

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
	std::pair<unsigned int, std::string>                  error_page;
	CheckServerInfoResult() : autoindex(false){};
};

class HttpServerInfoCheck {
  private:
	HttpServerInfoCheck();
	~HttpServerInfoCheck();

	static void CheckDTOServerInfo(
		CheckServerInfoResult    &result,
		const MockDtoServerInfos &server_info,
		HeaderFields             &header_fields
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

  public:
	static CheckServerInfoResult
	Check(const MockDtoServerInfos &server_info, HttpRequestFormat &request);
};

} // namespace http

#endif
