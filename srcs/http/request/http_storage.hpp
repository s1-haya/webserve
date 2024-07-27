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

struct ClientSaveData {
	// HTTP各書式のパースしたかどうか
	IsHttpRequestFormat save_is_request_format;
	// HttpRequestResult
	HttpRequestResult save_request_result;
	// HTTP各書式をパースする前の読み込んだ情報
	std::string save_current_buf;
};

class HttpStorage {
  public:
	// Create
	static void CreateClientSaveData(int client_fd);
	// Check
	static bool IsClientSaveData(int client_fd);
	// Get
	static const ClientSaveData &GetClientSaveData(int client_fd);
	// Update
	// Delete
	typedef std::map<int, ClientSaveData> ClientSaveDataMap;

  private:
	HttpStorage();
	~HttpStorage();
	// client_fd -> 前回保存した情報にアクセスするためのデータ構造
	static ClientSaveDataMap save_data_;
};

} // namespace http

#endif
