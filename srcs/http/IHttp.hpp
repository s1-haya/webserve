#ifndef IHTTP_HPP_
#define IHTTP_HPP_

#include "cgi.hpp"
#include "client_infos.hpp"
#include "error_state.hpp"
#include "virtual_server.hpp"

namespace server {

typedef std::list<const VirtualServer *> VirtualServerAddrList;

}

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
	Run(const ClientInfos &client_infos, const server::VirtualServerAddrList &virtual_servers) = 0;
	virtual HttpResult GetErrorResponse(const ClientInfos &client_info, ErrorState state)        = 0;
	virtual void       SetCgiResponse(int client_fd, const cgi::CgiResponse &cgi_response)     = 0;
};

} // namespace http

#endif /* IHTTP_HPP_ */
