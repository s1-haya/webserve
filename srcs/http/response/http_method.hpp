#ifndef HTTP_METHOD_HPP_
#define HTTP_METHOD_HPP_

#include "stat.hpp"
#include "status_code.hpp"
#include <list>

namespace utils {

class SystemException;

}

namespace http {

typedef std::map<std::string, std::string> HeaderFields;

class Method {
  public:
	static StatusCode Handler(
		const std::string            &path,
		const std::string            &method,
		const std::list<std::string> &allow_methods,
		const std::string            &request_body_message,
		std::string                  &response_body_message,
		HeaderFields                 &header_fields
	);

  private:
	static StatusCode
	GetHandler(const std::string &path, std::string &body_message, HeaderFields &header_fields);
	static StatusCode PostHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message,
		HeaderFields      &header_fields
	);
	static StatusCode DeleteHandler(
		const std::string &path, std::string &response_body_message, HeaderFields &header_fields
	);
	static Stat       TryStat(const std::string &path);
	static bool
	IsAllowedMethod(const std::string &method, const std::list<std::string> &allow_methods);
	static void       SystemExceptionHandler(const utils::SystemException &e);
	static StatusCode FileCreationHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message
	);
};

} // namespace http

#endif
