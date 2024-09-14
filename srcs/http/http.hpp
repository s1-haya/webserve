#ifndef HTTP_HPP_
#define HTTP_HPP_

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

class Http {
  public:
	Http();
	~Http();
	HttpResult
	Run(const MockDtoClientInfos &client_info, const server::VirtualServerAddrList &server_info);
	//  HttpResult GetErrorResponse(const ClientInfos &client_info, ErrState state);
	// For test
	HttpRequestParsedData GetClientData(int client_fd);

  private:
	Http(const Http &other);
	Http               &operator=(const Http &other);
	HttpStorage         storage_;
	utils::Result<void> ParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	std::string         CreateHttpResponse(
				const MockDtoClientInfos &client_info, const server::VirtualServerAddrList &server_info
			);
	bool IsHttpRequestFormatComplete(int client_fd);
	// HttpResult CreateBadRequestResponse(int client_fd);
};

} // namespace http

#endif
