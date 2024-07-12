#include "cgi.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace cgi {

struct CgiRequest {
	Cgi::MetaMap meta_variables;
	std::string  body_message;
};

class CgiParse {
  public:
	// todo 引数は要相談（クライアントとサーバーの情報を追加するかも）
	static CgiRequest Parse(const std::string &http_request);

  private:
	// todo メタ変数を作成するにはサーバーの情報、クライアントの情報、HTTPリクエスト情報が必要
	static Cgi::MetaMap CreateRequestMetaVariables();
}; // namespace cgi class CgiParse

} // namespace cgi
