#ifndef STATUS_CODE_HPP_
#define STATUS_CODE_HPP_

namespace http {

enum StatusCode {
	OK              = 200,
	BAD_REQUEST     = 400,
	NOT_FOUND       = 404,
	NOT_IMPLEMENTED = 501
};

}


#endif
