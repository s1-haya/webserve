#ifndef RESULT_HPP_
#define RESULT_HPP_

enum StatusCode {
	OK,
	NOT_FOUND
};

struct Result {
	StatusCode status_code;
	void *result;
};

#endif
