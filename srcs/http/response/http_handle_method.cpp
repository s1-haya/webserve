#include "http_response.hpp"
#include "stat.hpp"
#include "system_exception.hpp"
#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_serverinfo_check.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {

std::string FileToString(const std::ifstream &file) {
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

std::string ReadFile(const std::string &file_path) {
	std::ifstream file(file_path.c_str());
	if (!file) {
		// todo: default error page?
		std::ifstream error_file("html/404.html");
		return FileToString(error_file);
	}
	return FileToString(file);
}

} // namespace

// todo: 各メソッドを実行する関数
// returnがある場合、設定する
// - メソッドの権限の確認
//  -> 権限がない場合405 Not Allowed
// - 各メソッドを実行

namespace http {

void HttpResponse::MethodHandler(const CheckServerInfoResult &server_info, const std::string& method) {
	std::string request_message;
	std::string response_message;
	if (method == GET) {
		GetHandler(server_info.path, response_message);
	} else if (method == POST) {
		PostHandler(server_info.path, request_message, response_message);
	} else if (method == DELETE) {
		DeleteHandler(server_info.path, response_message);
	} else {
		throw HttpException("Error: Not Implemented", StatusCode(NOT_IMPLEMENTED));
	}
	(void)request_message;
	(void)response_message;
 }

void HttpResponse::GetHandler(const std::string &path, std::string &response_body_message) {
	try {
		Stat info(path);
		if (info.IsDirectory()) {
			// No empty string because the path has '/'
			if (path[path.size() - 1] != '/') {
				response_body_message = CreateDefaultBodyMessageFormat(StatusCode(MOVED_PERMANENTLY));
				return;
			}
			// todo: Check for index directive and handle ReadFile function
			// todo: Check for autoindex directive and handle AutoindexHandler function
			// todo: Return 403 Forbidden if neither index nor autoindex directives exist
		} else if (info.IsRegularFile()) {
			if (!info.IsReadableFile()) {
				response_body_message = CreateDefaultBodyMessageFormat(StatusCode(FORBIDDEN));
			} else {
				response_body_message = ReadFile(path);
			}
		} else {
			response_body_message = CreateDefaultBodyMessageFormat(StatusCode(NOT_FOUND));
		}
	} catch (const utils::SystemException &e) {
		SystemExceptionHandler(e, response_body_message);
	}
}

void HttpResponse::PostHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message
) {
	try {
		Stat info(path);
		if (info.IsDirectory()) {
			response_body_message = CreateDefaultBodyMessageFormat(StatusCode(FORBIDDEN));
		} else if (info.IsRegularFile()) {
			response_body_message = CreateDefaultBodyMessageFormat(StatusCode(NO_CONTENT));
		} else {
			// Location header fields: URI-reference
			// ex) POST /save/test.txt HTTP/1.1
			// Location: /save/test.txt;
			FileCreationHandler(path, request_body_message, response_body_message);
		}
	} catch (const utils::SystemException &e) {
		int error_number = e.GetErrorNumber();
		if (error_number == ENOENT) {
			FileCreationHandler(path, request_body_message, response_body_message);
		} else {
			SystemExceptionHandler(e, response_body_message);
		}
	}
}

void HttpResponse::DeleteHandler(const std::string &path, std::string &response_body_message) {
	try {
		Stat info(path);
		if (info.IsDirectory()) {
			response_body_message = CreateDefaultBodyMessageFormat(StatusCode(FORBIDDEN));
		} else if (std::remove(path.c_str()) == 0) {
			response_body_message = CreateDefaultBodyMessageFormat(StatusCode(NO_CONTENT));
		} else {
			throw utils::SystemException(std::strerror(errno), errno);
		}
	} catch (const utils::SystemException &e) {
		SystemExceptionHandler(e, response_body_message);
	}
}

void HttpResponse::SystemExceptionHandler(
	const utils::SystemException &e, std::string &response_body_message
) {
	int error_number = e.GetErrorNumber();
	if (error_number == EACCES || error_number == EPERM) {
		response_body_message = CreateDefaultBodyMessageFormat(StatusCode(FORBIDDEN));
	} else if (error_number == ENOENT || error_number == ENOTDIR || error_number == ELOOP || error_number == ENAMETOOLONG) {
		response_body_message = CreateDefaultBodyMessageFormat(StatusCode(NOT_FOUND));
	} else {
		response_body_message = CreateDefaultBodyMessageFormat(StatusCode(INTERNAL_SERVER_ERROR));
	}
}

void HttpResponse::FileCreationHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message
) {
	std::ofstream file(path.c_str(), std::ios::binary);
	if (file.fail()) {
		response_body_message = CreateDefaultBodyMessageFormat(StatusCode(FORBIDDEN));
		return;
	}
	file.write(request_body_message.c_str(), request_body_message.length());
	file.close();
	if (file.fail()) {
		if (std::remove(path.c_str()) != 0) {
			throw utils::SystemException(std::strerror(errno), errno);
		}
		response_body_message = CreateDefaultBodyMessageFormat(StatusCode(FORBIDDEN));
		return;
	}
	response_body_message = CreateDefaultBodyMessageFormat(StatusCode(CREATED));
}

} // namespace http
