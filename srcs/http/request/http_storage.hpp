#ifndef HTTP_STORAGE_HPP_
#define HTTP_STORAGE_HPP_

#include "http_parse.hpp"
#include <map>

namespace http {

class HttpStorage {
  public:
	HttpStorage();
	~HttpStorage();
	// Get
	const HttpRequestParsedData &GetClientSaveData(int client_fd);
	// Update
	void UpdateClientSaveData(int client_fd, const HttpRequestParsedData &client_data);
	// Delete
	void DeleteClientSaveData(int client_fd);

  private:
	HttpStorage(const HttpStorage &other);
	HttpStorage &operator=(const HttpStorage &other);
	// client_fd -> 前回保存した情報にアクセスするためのデータ構造
	typedef std::map<int, HttpRequestParsedData> ClientSaveDataMap;
	ClientSaveDataMap                     save_data_;
	// Create
	void CreateClientSaveData(int client_fd);
	// Check
	bool IsClientSaveData(int client_fd);
};

} // namespace http

#endif
