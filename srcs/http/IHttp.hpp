#ifndef IHTTP_HPP_
#define IHTTP_HPP_

#include "server/dto/dto_server_to_http.hpp"

namespace http {

struct HttpResult;

class IHttp {
  public:
	virtual ~IHttp() {}

	// todo: update params
	/**
	 * @brief Processes the HTTP request and generates a response.
	 *
	 * This method handles the incoming HTTP request, performs the necessary
	 * operations, and produces an appropriate response.
	 *
	 * @param client_infos(tmp)
	 * @param virtual_servers(tmp)
	 *
	 * @return HttpResult indicating whether the response is complete
	 *         and containing the response data.
	 */
	virtual HttpResult
						Run(const server::DtoClientInfos        &client_infos,
							const server::VirtualServerAddrList &virtual_servers) = 0;
	virtual std::string GetTimeoutResponse(int client_fd)                         = 0;
};

} // namespace http

#endif /* IHTTP_HPP_ */
