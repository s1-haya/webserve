#ifndef MOCK_HTTP_HPP_
#define MOCK_HTTP_HPP_

#include "IHttp.hpp"
#include <map>

namespace http {

struct HttpResult;

class MockHttp : public IHttp {
  public:
	enum MessageType {
		HTTP_METHOD,
		HTTP_REQUEST_TARGET,
		HTTP_VERSION,
		HTTP_CONTENT,
		HTTP_STATUS,
		HTTP_STATUS_TEXT
	};
	typedef std::map<MessageType, std::string> RequestMessage;
	MockHttp();
	~MockHttp();
	// override
	HttpResult
	Run(const ClientInfos &client_infos, const server::VirtualServerAddrList &virtual_servers);
	HttpResult GetErrorResponse(const ClientInfos &client_info, ErrState state);
	// todo: CgiResponseを受け取ってセットする。struct CgiResponseでIHttpにも追加？
	void SetCgiResponse(int client_fd, const std::string &cgi_response);

  private:
	// prohibit copy
	MockHttp(const MockHttp &other);
	MockHttp &operator=(const MockHttp &other);
	void      ParseRequest(const std::string &read_buf);
	void      ReadPathContent();
	// variable
	RequestMessage request_;
};

} // namespace http

#endif /* MOCK_HTTP_HPP_ */
