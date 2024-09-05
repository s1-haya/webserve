#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_response.hpp"
#include "http_serverinfo_check.hpp"
#include "stat.hpp"
#include "system_exception.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h> // access

namespace {

bool IsExistPath(const std::string &path) {
	return access(path.c_str(), F_OK) == 0;
}

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

StatusCode
HttpResponse::MethodHandler(const CheckServerInfoResult &server_info, const std::string &method) {
	StatusCode  status_code(OK);
	std::string request_message;
	std::string response_message;
	(void)request_message;
	(void)response_message;
	// 引数: path, allow_method, method, request_message, response_message
	if (method == GET) {
		status_code = GetHandler(server_info.path, response_message);
	} else if (method == POST) {
		status_code = PostHandler(server_info.path, request_message, response_message);
	} else if (method == DELETE) {
		status_code = DeleteHandler(server_info.path, response_message);
	} else {
		status_code      = StatusCode(NOT_IMPLEMENTED);
		response_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Not Implemented", status_code);
	}
	return status_code;
}

// todo: refactor
StatusCode HttpResponse::GetHandler(const std::string &path, std::string &response_body_message) {
	StatusCode status_code(OK);
	Stat       info = TryStat(path, response_body_message);
	if (info.IsDirectory()) {
		// No empty string because the path has '/'
		if (path[path.size() - 1] != '/') {
			status_code           = StatusCode(MOVED_PERMANENTLY);
			response_body_message = CreateDefaultBodyMessageFormat(status_code);
			throw HttpException("Error: Moved Permanently", status_code);
		}
		// todo: Check for index directive and handle ReadFile function
		// todo: Check for autoindex directive and handle AutoindexHandler function
		// todo: Return 403 Forbidden if neither index nor autoindex directives exist
	} else if (info.IsRegularFile()) {
		if (!info.IsReadableFile()) {
			status_code           = StatusCode(FORBIDDEN);
			response_body_message = CreateDefaultBodyMessageFormat(status_code);
			throw HttpException("Error: Forbidden", status_code);
		} else {
			response_body_message = ReadFile(path);
		}
	} else {
		status_code           = StatusCode(NOT_FOUND);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Not Found", status_code);
	}
	return status_code;
}

StatusCode HttpResponse::PostHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message
) {
	if (!IsExistPath(path)) {
		return FileCreationHandler(path, request_body_message, response_body_message);
	}
	const Stat &info = TryStat(path, response_body_message);
	StatusCode  status_code(NO_CONTENT);
	if (info.IsDirectory()) {
		status_code           = StatusCode(FORBIDDEN);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Forbidden", status_code);
	} else if (info.IsRegularFile()) {
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
	} else {
		// Location header fields: URI-reference
		// ex) POST /save/test.txt HTTP/1.1
		// Location: /save/test.txt;
		status_code = FileCreationHandler(path, request_body_message, response_body_message);
	}
	return status_code;
}

StatusCode
HttpResponse::DeleteHandler(const std::string &path, std::string &response_body_message) {
	const Stat &info        = TryStat(path, response_body_message);
	StatusCode  status_code = StatusCode(NO_CONTENT);
	if (info.IsDirectory()) {
		status_code           = StatusCode(FORBIDDEN);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Forbidden", status_code);
	} else if (std::remove(path.c_str()) == 0) {
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
	} else {
		throw utils::SystemException(std::strerror(errno), errno);
	}
	return status_code;
}

void HttpResponse::SystemExceptionHandler(
	const utils::SystemException &e, std::string &response_body_message
) {
	StatusCode status_code  = StatusCode(INTERNAL_SERVER_ERROR);
	int        error_number = e.GetErrorNumber();
	if (error_number == EACCES || error_number == EPERM) {
		status_code           = StatusCode(FORBIDDEN);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Forbidden", status_code);
	} else if (error_number == ENOENT || error_number == ENOTDIR || error_number == ELOOP ||
			   error_number == ENAMETOOLONG) {
		status_code           = StatusCode(NOT_FOUND);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Not Found", status_code);
	} else {
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Internal Server Error", status_code);
	}
}

StatusCode HttpResponse::FileCreationHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message
) {
	StatusCode    status_code(CREATED);
	std::ofstream file(path.c_str(), std::ios::binary);
	if (file.fail()) {
		status_code           = StatusCode(FORBIDDEN);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Forbidden", status_code);
	}
	file.write(request_body_message.c_str(), request_body_message.length());
	file.close();
	if (file.fail()) {
		if (std::remove(path.c_str()) != 0) {
			throw utils::SystemException(std::strerror(errno), errno);
		}
		status_code           = StatusCode(FORBIDDEN);
		response_body_message = CreateDefaultBodyMessageFormat(status_code);
		throw HttpException("Error: Forbidden", status_code);
	}
	response_body_message = CreateDefaultBodyMessageFormat(status_code);
	return status_code;
}

Stat HttpResponse::TryStat(const std::string &path, std::string &response_body_message) {
	struct stat stat_buf;
	try {
		if (stat(path.c_str(), &stat_buf) == -1) {
			std::string error_message =
				"Error: stat on path '" + path + "': " + std::strerror(errno);
			throw utils::SystemException(error_message, errno);
		}
	} catch (const utils::SystemException &e) {
		SystemExceptionHandler(e, response_body_message);
	}
	Stat info(stat_buf);
	return info;
}

} // namespace http
