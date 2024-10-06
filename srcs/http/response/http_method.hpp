#ifndef HTTP_METHOD_HPP_
#define HTTP_METHOD_HPP_

#include "stat.hpp"
#include "status_code.hpp"
#include "utils.hpp"
#include <list>

namespace http {

typedef std::map<std::string, std::string> HeaderFields;

class Method {
  public:
	typedef std::list<std::string> AllowMethods;
	static StatusCode              Handler(
					 const std::string  &path,
					 const std::string  &method,
					 const AllowMethods &allow_methods,
					 const std::string  &request_body_message,
					 std::string        &response_body_message,
					 HeaderFields       &response_header_fields,
					 const std::string  &index_file_path,
					 bool                autoindex_on,
					 const std::string  &upload_directory
				 );
	static bool IsSupportedMethod(const std::string &method);
	static bool
	IsAllowedMethod(const std::string &method, const std::list<std::string> &allow_methods);

  private:
	static StatusCode GetHandler(
		const std::string &path,
		std::string       &body_message,
		HeaderFields      &response_header_fields,
		const std::string &index_file_path,
		bool               autoindex_on
	);
	static StatusCode PostHandler(
		const std::string &path,
		const std::string &request_body_message,
		std::string       &response_body_message,
		HeaderFields      &response_header_fields,
		const std::string &upload_directory
	);
	static StatusCode DeleteHandler(
		const std::string &path,
		std::string       &response_body_message,
		HeaderFields      &response_header_fields
	);
	static Stat        TryStat(const std::string &path);
	static std::string ReadFile(const std::string &file_path);
	static void        SystemExceptionHandler(int error_number);
	static StatusCode  FileCreationHandler(
		 const std::string &path,
		 const std::string &request_body_message,
		 std::string       &response_body_message,
		 HeaderFields      &response_header_fields
	 );
	static StatusCode EchoPostHandler(
		const std::string &request_body_message,
		std::string       &response_body_message,
		HeaderFields      &response_header_fields
	);
	static utils::Result<std::string> AutoindexHandler(const std::string &path);

	// マルチパート用のパートを表す構造体
	struct Part {
		std::map<std::string, std::string> headers;
		std::string                        body;
	};
	// multipart/form-dataをデコードする関数
	std::string              ExtractBoundary(const std::string &content_type);
	std::vector<std::string> SplitParts(const std::string &body, const std::string &boundary);
	Part                     ParsePart(const std::string &part);
	std::vector<Part>
	DecodeMultipartFormData(const std::string &content_type, const std::string &body);
};

} // namespace http

#endif
