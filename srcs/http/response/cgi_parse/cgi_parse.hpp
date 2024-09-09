#include "cgi.hpp"
#include "http_format.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace http {
namespace cgi {

struct CgiRequest {
	Cgi::MetaMap meta_variables;
	std::string  body_message;
};

class CgiParse {
  public:
	static CgiRequest Parse(const HttpRequestFormat &request);

  private:
	CgiParse();
	~CgiParse();
	CgiParse(const CgiParse &other);
	CgiParse &operator=(const CgiParse &other);
	// todo メタ変数を作成するにはサーバーの情報、クライアントの情報、HTTPリクエスト情報が必要
	static Cgi::MetaMap CreateRequestMetaVariables();
}; // namespace cgi class CgiParse

} // namespace cgi
} // namespace http
