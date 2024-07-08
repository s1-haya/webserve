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

// HTTPリクエストをパースしてCGIRequestを作成するクラス
class CGIParse {
  public:
	// 引数は要相談（クライアントとサーバーの情報を追加するかも）
	CGIRequest parse(const std::string &http_request);

  private:
	CGI::MetaMap create_request_meta_variables();
}; // namespace cgi class CGIParse
} // namespace cgi
