#ifndef HTTP_STORAGE_HPP_
#define HTTP_STORAGE_HPP_

#include "http_parse.hpp"
#include <map>

namespace http {

// todo: 今後http_parse.hppに移動する
struct IsHttpRequestFormat {
	IsHttpRequestFormat()
		: is_request_line(false), is_header_fields(false), is_body_message(false){};
	bool is_request_line;
	bool is_header_fields;
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
	HttpStorage();
	~HttpStorage();
	// Get
	const ClientSaveData &GetClientSaveData(int client_fd);
	// Update
	// Delete

  private:
	HttpStorage(const HttpStorage &other);
	HttpStorage                          &operator=(const HttpStorage &other);
	typedef std::map<int, ClientSaveData> ClientSaveDataMap;
	// client_fd -> 前回保存した情報にアクセスするためのデータ構造
	ClientSaveDataMap save_data_;
	// Create
	void CreateClientSaveData(int client_fd);
	// Check
	bool IsClientSaveData(int client_fd);
};

} // namespace http

#endif
