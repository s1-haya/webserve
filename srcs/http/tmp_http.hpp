#ifndef TMP_HTTP_HPP_
#define TMP_HTTP_HPP_

#include "http_parse.hpp"
#include "http_storage.hpp"

namespace http {

class TmpHttp {
  public:
	TmpHttp();
	~TmpHttp();
	void        ParseHttpRequestFormat(int client_fd, const std::string &read_buf);
	std::string CreateHttpResponse(int client_fd);
	bool        GetIsHttpRequestFormatComplete(int client_fd);
	// todo: 408のtimeoutのレスポンス

  private:
	TmpHttp(const TmpHttp &other);
	TmpHttp    &operator=(const TmpHttp &other);
	HttpStorage storage_;
};

} // namespace http

#endif
