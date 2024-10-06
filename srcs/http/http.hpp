#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "IHttp.hpp"
#include "http_parse.hpp"
#include "http_response.hpp"
#include "http_storage.hpp"
#include "result.hpp"

namespace http {

struct ClientInfos;
struct HttpResult;

class Http : public IHttp {
  public:
	Http();
	~Http();
	HttpResult
	Run(const ClientInfos &client_info, const server::VirtualServerAddrList &server_info);
	HttpResult GetErrorResponse(int client_fd, ErrorState state);
	HttpResult GetResponseFromCgi(int client_fd, const cgi::CgiResponse &cgi_response);

  private:
	Http(const Http &other);
	Http               &operator=(const Http &other);
	HttpStorage         storage_;
	utils::Result<void> ParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	HttpResult          CreateHttpResponse(
				 const ClientInfos &client_info, const server::VirtualServerAddrList &server_info
			 );
	HttpResult            CreateBadRequestResponse(int client_fd);
	bool                  IsHttpRequestFormatComplete(int client_fd);
	HttpRequestParsedData GetClientData(int client_fd);
};

} // namespace http

#endif
