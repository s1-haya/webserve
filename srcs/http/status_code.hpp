#ifndef STATUS_CODE_HPP_
#define STATUS_CODE_HPP_

#include <map>
#include <string>

namespace http {

enum EStatusCode {
	OK                    = 200,
	CREATED               = 201,
	NO_CONTENT            = 204,
	MOVED_PERMANENTLY     = 301,
	BAD_REQUEST           = 400,
	FORBIDDEN             = 403,
	NOT_FOUND             = 404,
	PAYLOAD_TOO_LARGE     = 413,
	INTERNAL_SERVER_ERROR = 500,
	NOT_IMPLEMENTED       = 501
};

class StatusCode {
  public:
	explicit StatusCode(EStatusCode status_code);
	~StatusCode();
	StatusCode(const StatusCode &other);
	StatusCode &operator=(const StatusCode &other);

	EStatusCode        GetEStatusCode() const;
	const std::string &GetStatusCode() const;
	const std::string &GetReasonPhrase() const;

  private:
	EStatusCode                                status_code_;
	std::string                                str_status_code_;
	typedef std::map<EStatusCode, std::string> ReasonPhrase;
	ReasonPhrase                               reason_phrase_;
	StatusCode::ReasonPhrase                   InitReasonPhrase();
};

} // namespace http

#endif
