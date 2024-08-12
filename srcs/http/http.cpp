#include "http.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "utils.hpp"
#include <fstream>
#include <sstream>
#include <vector>

namespace http {

Http::Http(const std::string &read_buf) {
	ParseRequest(read_buf);
	ReadPathContent();
}

Http::~Http() {}

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

// todo: tmp request_
void Http::ParseRequest(const std::string &read_buf) {
	const std::vector<std::string> lines      = utils::SplitStr(read_buf, CRLF);
	const std::string              start_line = lines[0];

	const std::vector<std::string> request_line = utils::SplitStr(start_line, SP);
	// set request-line(method, request-target, HTTP-version)
	request_[HTTP_METHOD]         = request_line[0];
	request_[HTTP_REQUEST_TARGET] = CreateDefaultPath(request_line[1]);
	request_[HTTP_VERSION]        = request_line[2];
}

namespace {

std::string FileToString(const std::ifstream &file) {
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

std::string ReadFile(const std::string &file_path) {
	std::ifstream file(file_path.c_str());
	if (!file) {
		std::ifstream error_file("html/404.html");
		utils::Debug("http", "404 file not found");
		return FileToString(error_file);
	}
	return FileToString(file);
}

} // namespace

// todo: tmp content
void Http::ReadPathContent() {
	const std::string content  = ReadFile(request_[HTTP_REQUEST_TARGET]);
	request_[HTTP_CONTENT]     = content;
	request_[HTTP_STATUS]      = "200";
	request_[HTTP_STATUS_TEXT] = "OK";
}

namespace {

void CreateStatusLine(std::ostream &response_stream, Http::RequestMessage &request) {
	response_stream << HTTP_VERSION << SP << request[Http::HTTP_STATUS] << SP
					<< request[Http::HTTP_STATUS_TEXT] << CRLF;
}

template <typename T>
void CreateHeaderField(std::ostream &response_stream, const std::string &name, const T &value) {
	response_stream << name << ":" << SP << value << SP << CRLF;
}

void CreateHeaderFields(std::ostream &response_stream, Http::RequestMessage &request) {
	CreateHeaderField(response_stream, CONNECTION, "close");
	CreateHeaderField(response_stream, CONTENT_LENGTH, request[Http::HTTP_CONTENT].size());
}

void CreateCRLF(std::ostream &response_stream) {
	response_stream << CRLF;
}

void CreateBody(std::ostream &response_stream, Http::RequestMessage &request) {
	response_stream << request[Http::HTTP_CONTENT];
}

} // namespace

HttpResult Http::CreateResponse() {
	std::ostringstream response_stream;
	CreateStatusLine(response_stream, this->request_);
	CreateHeaderFields(response_stream, this->request_);
	CreateCRLF(response_stream);
	CreateBody(response_stream, this->request_);

	HttpResult result;
	result.is_response_complete = true;
	result.response             = response_stream.str();
	return result;
}

} // namespace http
