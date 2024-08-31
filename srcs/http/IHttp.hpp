#ifndef IHTTP_HPP_
#define IHTTP_HPP_

#include "client_infos.hpp"
#include "server_infos.hpp"

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
	 * @param server_infos(tmp)
	 *
	 * @return HttpResult indicating whether the response is complete
	 *         and containing the response data.
	 */
	virtual HttpResult
	Run(const MockDtoClientInfos &client_infos, const MockDtoServerInfos &server_infos) = 0;
	virtual std::string GetTimeoutResponse(int client_fd)                               = 0;
};

} // namespace http

#endif /* IHTTP_HPP_ */
