#include <iostream>
#include <map>
#include <sstream>
#include <string>

// CGIリクエスト情報を持つ構造体
struct CGIRequest {
	std::map<std::string, std::string> meta_variables;
	std::string                        body_message;

	CGIRequest() : meta_variables(), body_message("") {}
};

// CGIリクエスト情報をパースするクラス
class CGIParse {
  public:
	CGIRequest parse(const std::string &http_request);

  private:
	std::map<std::string, std::string> parse_headers(const std::string &header_section);
	std::string get_value_from_header(const std::string &header, const std::string &key);
};
