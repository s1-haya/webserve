#include "http_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <vector>

namespace http {
namespace {

// todo: create path (/, /aaa, /aaa/)
std::string CreateDefaultPath(const std::string &path) {
	static const std::string location = "html";

	if (path.size() == 1) {
		return location + "/index.html";
	}
	return location + path + "/index.html";
}

} // namespace

HTTPParse::HTTPParse() {}

HTTPParse::~HTTPParse() {}

// todo: tmp request_
HTTPRequest HTTPParse::Run(const std::string &read_buf) {
	HTTPRequest                    request;
	const std::vector<std::string> lines      = utils::SplitStr(read_buf, CRLF);
	const std::string              start_line = lines[0];

	const std::vector<std::string> request_line = utils::SplitStr(start_line, SP);
	// set request-line(method, request-target, HTTP-version)
	request.status_line.method  = request_line[0];
	request.status_line.uri     = CreateDefaultPath(request_line[1]);
	request.status_line.version = request_line[2];
	return request;
}

} // namespace http
