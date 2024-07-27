#ifndef HTTP_HPP_
#define HTTP_HPP_

#include "http_storage.hpp"
#include "http_parse.hpp"

namespace http {

class Http {
	public:
		Http();
		~Http();
		void Run(int client_fd, const std::string& read_buf);

	private:
		Http(const Http& other);
		Http &operator=(const Http& other);
		static HttpStorage storage_;
		// todo: parse class
		static HttpParse parse_;
		// todo: response class
};

}

#endif
