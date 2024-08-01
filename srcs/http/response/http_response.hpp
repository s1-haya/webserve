#ifndef HTTP_RESPONSE_HPP_
#define HTTP_RESPONSE_HPP_

#include <string>

namespace http {

class HttpResponse {
  public:
	static std::string TmpRun(int client_fd);

  private:
	HttpResponse();
	~HttpResponse();
};

} // namespace http

#endif
