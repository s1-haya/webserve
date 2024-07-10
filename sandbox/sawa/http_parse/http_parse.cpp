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

void PrintLines(const std::vector<std::string> &lines) {
	typedef std::vector<std::string>::const_iterator It;
	size_t                                           i = 0;
	for (It it = lines.begin(); it != lines.end(); ++it) {
		// if (*it == "") {
		// 	std::cout << "stop!" << std::endl;
		// 	break;
		// }
		std::cout << i << ":" << *it << std::endl;
		i++;
	}
}

} // namespace

HTTPParse::HTTPParse() {}

HTTPParse::~HTTPParse() {}

StatusLine HTTPParse::SetStatusLine(const std::vector<std::string> &request_line) {
	StatusLine status_line;
	status_line.method  = request_line[0];
	status_line.uri     = CreateDefaultPath(request_line[1]);
	status_line.version = request_line[2];
	return status_line;
}

// todo: tmp request_
HTTPRequest HTTPParse::Run(const std::string &read_buf) {
	HTTPRequest                    request;
	const std::vector<std::string> lines = utils::SplitStr(read_buf, CRLF);
	PrintLines(lines);
	request.status_line = SetStatusLine(utils::SplitStr(lines[0], SP));
	return request;
}

} // namespace http
