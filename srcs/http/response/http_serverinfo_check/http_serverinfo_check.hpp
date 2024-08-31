#ifndef HTTP_SERVERINFO_CHECK_HPP_
#define HTTP_SERVERINFO_CHECK_HPP_

#include "http_format.hpp"
#include "server_infos.hpp"
#include <list>
#include <string>

namespace http {

struct CheckServerInfoResult {
	enum CheckStatus {
		CONTINUE,
		REDIRECT_ON
	}; // rfc + 見やすいように独自で名前をつけた
	// 呼び出し元でこれをチェックしてstatus codeを付ける用

	std::string path; // alias, index, redirectを見る
	std::string index;
	bool        autoindex;

	// メソッドの処理で使用
	std::list<std::string> allowed_methods;
	std::string            cgi_extension;
	std::string            upload_directory;

	unsigned int                         redirect_status_code;
	std::pair<unsigned int, std::string> error_page;
	CheckStatus                          status;
	CheckServerInfoResult() : autoindex(false), redirect_status_code(0), status(CONTINUE){};
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
