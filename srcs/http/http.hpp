#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "debug.hpp" // todo: tmp
#include <string>

class Http {
  public:
	// todo: tmp
	Http();
	~Http();

  private:
	// prohibit copy
	Http(const Http &other);
	Http &operator=(const Http &other);
};

#endif /* HTTP_HPP_ */
