#ifndef TMP_HTTP_HPP_
#define TMP_HTTP_HPP_

#include "client_info.hpp"
#include "http_parse.hpp"
#include "http_response.hpp"
#include "http_storage.hpp"
#include "result.hpp"
#include "server_info.hpp"

namespace http {

// todo: server
// Http class {
// public
// - Run: リクエスト受信からクライアント作成まで
// - TimeOut: クライアントのリクエストが一定時間送信されない時
// - InternalServer: システムコール等が失敗した場合

// private
// - HttpRequest
// - HttpResponse
// }
struct HttpResult;

class TmpHttp {
  public:
	TmpHttp();
	~TmpHttp();
	HttpResult
					   Run(const server::ClientInfo &client_info,
						   const server::ServerInfo &server_info,
						   const std::string        &read_buf);
	void               ParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	utils::Result<int> TmpParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	std::string        CreateHttpResponse(int client_fd);
	bool               GetIsHttpRequestFormatComplete(int client_fd);
	// todo: 408のtimeoutのレスポンス

	// For test
	HttpRequestParsedData GetClientData(int client_fd);

  private:
	TmpHttp(const TmpHttp &other);
	TmpHttp    &operator=(const TmpHttp &other);
	HttpStorage storage_;
};

} // namespace http

#endif
