#ifndef HTTP_SERVERINFO_CHECK_HPP_
#define HTTP_SERVERINFO_CHECK_HPP_

#include "http_parse.hpp"
#include <string>

// Mock(Serverの構造体に未実装の部分があるため)
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

#include <list>
struct MockLocationCon { /*一部serverで未実装*/
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect; // cannot use return
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

namespace http {

struct CheckPathResult {
	enum CheckStatus {
		OK,
		INVALID_HOST,
		PAYLOAD_TOO_LARGE,
		LOCATION_NOT_FOUND,
		REDIRECT
	}; // rfc + 見やすいように独自で名前をつけた
	// 呼び出し元でこれをチェックしてstatus codeを付ける用

	std::string path; // alias, index, redirectを見る
	std::string index;
	bool        autoindex;
	int         status_code; // redirectで指定
	std::string error_page_path;
	int         error_status_code; // error_pageで指定 まとめる？
	CheckStatus is_ok;
	CheckPathResult() : autoindex(false), status_code(0), error_status_code(0), is_ok(OK) {};
};

class HttpPathCheck {
  private:
	HttpPathCheck();
	~HttpPathCheck();

	static void CheckDTOServerInfo(
		CheckPathResult &result, const MockDtoServerInfos &server_info, HeaderFields &header_fields
	);
	static void CheckLocationList(
		CheckPathResult &result, const LocationList &locations, const std::string &request_target
	);
	static MockLocationCon CheckLocation(
		CheckPathResult &result, const LocationList &locations, const std::string &request_target
	);
	static void CheckIndex(CheckPathResult &result, const MockLocationCon &location);
	static void CheckAutoIndex(CheckPathResult &result, const MockLocationCon &location);
	static void CheckAlias(CheckPathResult &result, const MockLocationCon &location);
	static void CheckRedirect(CheckPathResult &result, const MockLocationCon &location);

  public:
	static CheckPathResult Check(const MockDtoServerInfos &server_info, HttpRequest &request);
};

} // namespace http

#endif
