#ifndef TMP_HTTP_HPP_
#define TMP_HTTP_HPP_

#include "http_parse.hpp"
#include "http_response.hpp"
#include "http_storage.hpp"
#include "result.hpp"

namespace server {

struct DtoClientInfos;
struct DtoServerInfos;

} // namespace server

namespace http {

// todo: server
// Http class {
// public
// - Run: リクエスト受信からクライアント作成まで
// - TimeOut: クライアントのリクエストが一定時間送信されない時
// - InternalServer: システムコール等が失敗した場合

// private
// - HttpRequestFormat
// - HttpResponse
// }
struct HttpResult;

class TmpHttp {
  public:
	TmpHttp();
	~TmpHttp();
	HttpResult Run(const MockDtoClientInfos &client_info, const MockDtoServerInfos &server_info);
	void       ParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	utils::Result<void> TmpParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	std::string         CreateHttpResponse(int client_fd);
	std::string         CreateHttpResponse(
				const MockDtoClientInfos &client_info, const MockDtoServerInfos &server_info
			);
	// todo: 408のtimeoutのレスポンス

	// For test
	HttpRequestParsedData GetClientData(int client_fd);

  private:
	TmpHttp(const TmpHttp &other);
	TmpHttp    &operator=(const TmpHttp &other);
	HttpStorage storage_;
	bool        IsHttpRequestFormatComplete(int client_fd);
};

} // namespace http

#endif
