#ifndef HTTP_HPP_
#define HTTP_HPP_

namespace http {

class Http {
	public:
		Http();
		~Http();
	private:
		Http(const Http& other);
		Http &operator=(const Http& other);
};

}

#endif
