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
	Run(const server::DtoClientInfos &client_infos, const server::DtoServerInfos &server_infos);
	std::string GetTimeoutResponse(int client_fd);

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
