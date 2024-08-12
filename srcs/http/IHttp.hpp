#ifndef IHTTP_HPP_
#define IHTTP_HPP_

namespace http {

struct HttpResult;

class IHttp {
  public:
	virtual ~IHttp() {}
	virtual HttpResult CreateResponse() = 0;
};

} // namespace http

#endif /* IHTTP_HPP_ */
