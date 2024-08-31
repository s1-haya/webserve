#ifndef HTTP_PATH_CHECK_HPP_
#define HTTP_PATH_CHECK_HPP_

#include <list>
#include <map>
#include <string>

// Mock(Serverの構造体に未実装の部分があるため)
/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

struct LocationCon { /*一部serverで未実装*/
	std::string                          request_uri;
	std::string                          alias;
	std::string                          index;
	bool                                 autoindex;
	std::list<std::string>               allowed_methods;
	std::pair<unsigned int, std::string> redirect; // cannnot use return
	LocationCon() : autoindex(false) {}
};

typedef std::list<int>         PortList;
typedef std::list<LocationCon> LocationList;

struct DtoServerInfos {
	int                    server_fd;
	std::list<std::string> server_names;
	std::string            server_port;
	LocationList           locations;
	/*以下serverでは未実装*/
	std::string                          host;
	std::size_t                          client_max_body_size;
	std::pair<unsigned int, std::string> error_page;
};

struct RequestLine {
	std::string method;
	std::string request_target;
	std::string version;
};

typedef std::map<std::string, std::string> HeaderFields;

struct HttpRequestFormat {
	RequestLine  request_line;
	HeaderFields header_fields;
	std::string  body_message;
};

/*-------------------------------------------------------------*/
/*-------------------------------------------------------------*/

namespace http {

enum CheckStatus {
	OK,
	INVALID_HOST,
	PAYLOAD_TOO_LARGE,
	LOCATION_NOT_FOUND,
	REDIRECT
}; // rfc + 見やすいように独自で名前をつけた
// 呼び出し元でこれをチェックしてstatus codeを付ける用

struct CheckPathResult {
	std::string path; // alias, index, redirectを見る
	std::string index;
	bool        autoindex;
	int         status_code; // redirectで指定
	std::string error_page_path;
	int         error_status_code; // error_pageで指定 まとめる？
	CheckStatus is_ok;
	CheckPathResult() : autoindex(false), status_code(0), error_status_code(0), is_ok(OK){};
};

class HttpPathCheck {
  private:
	HttpPathCheck();
	~HttpPathCheck();

	static void CheckDTOServerInfo(
		CheckPathResult &result, const DtoServerInfos &server_info, HeaderFields &header_fields
	);
	static void CheckLocationList(
		CheckPathResult &result, const LocationList &locations, const std::string &request_target
	);
	static LocationCon CheckLocation(
		CheckPathResult &result, const LocationList &locations, const std::string &request_target
	);
	static void CheckIndex(CheckPathResult &result, const LocationCon &location);
	static void CheckAutoIndex(CheckPathResult &result, const LocationCon &location);
	static void CheckAlias(CheckPathResult &result, const LocationCon &location);
	static void CheckRedirect(CheckPathResult &result, const LocationCon &location);

  public:
	static CheckPathResult Check(const DtoServerInfos &server_info, HttpRequestFormat &request);
};

} // namespace http

#endif
