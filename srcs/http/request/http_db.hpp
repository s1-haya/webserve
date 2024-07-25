#ifndef HTTP_DB_HPP_
#define HTTP_DB_HPP_

#include "http_parse.hpp"
#include <map>

namespace http {

// todo: 今後http_parse.hppに移動する
struct  IsHttpRequestFormat
{
	IsHttpRequestFormat() : is_response(false), is_request_line(false), is_header_fileds(false), is_body_message(false) {};
	// is_response: server側にレスポンスを作成できるかのbool値
	bool is_response;
	bool is_request_line;
	bool is_header_fileds;
	bool is_body_message;
};

struct SaveData
{
	// HTTP各書式のパースしたかどうか
	IsHttpRequestFormat save_is_request_format;
	// HttpRequestResult
	HttpRequestResult save_request_result; 
};


class HttpDb {
	public:
		// Create
		explicit HttpDb(int client_fd);
		// Read
		// Update
		// Delete
	private:
	  // client_fd -> 前回保存した情報にアクセスするためのデータ構造
	  std::map<int, SaveData> save_data_;
};

}

#endif
