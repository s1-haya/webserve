#ifndef HTTP_DB_HPP_
#define HTTP_DB_HPP_

#include "http_parse.hpp"
#include <map>

namespace http {

// todo: 今後http_parse.hppに移動する
struct IsHttpRequestFormat {
	IsHttpRequestFormat()
		: is_response(false),
		  is_request_line(false),
		  is_header_fileds(false),
		  is_body_message(false){};
	// is_response: server側にレスポンスを作成できるかのbool値
	bool is_response;
	bool is_request_line;
	bool is_header_fileds;
	bool is_body_message;
};

struct SaveData {
	// HTTP各書式のパースしたかどうか
	IsHttpRequestFormat save_is_request_format;
	// HttpRequestResult
	HttpRequestResult save_request_result;
	// 現在HTTP各書式のパース前の読み込んだ情報
	const std::string& current_buf;
};

class HttpDb {
  public:
	// Create
	static void CreateSaveData(int client_fd);
	// Check
	static bool IsSaveData(int client_fd);
	// Get
	static const SaveData &GetSaveData(int client_fd);
	// Update
	// Delete
  private:
	HttpDb();
	~HttpDb();
	// client_fd -> 前回保存した情報にアクセスするためのデータ構造
	static std::map<int, SaveData> save_data_;
};

} // namespace http

#endif
