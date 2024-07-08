#include "cgi.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace cgi {

// メタ変数を作成するにはサーバーの情報、クライアントの情報、HTTPリクエスト情報が必要
// CGIリクエスト情報を持つ構造体
struct CGIRequest {
	CGI::MetaMap meta_variables;
	std::string  body_message;
};

// CGIリクエスト情報をパースするクラス
class CGIParse {
  public:
	CGIRequest parse(const std::string &http_request);

  private:
	std::map<std::string, std::string> parse_headers(const std::string &header_section);
	std::string get_value_from_header(const std::string &header, const std::string &key);
}; // namespace cgi class CGIParse
} // namespace cgi
