#include "http_response.hpp"
#include "stat.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <unistd.h>

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

void HttpResponse::GetHandler(const std::string &path, std::string &response_body_message) {
	try {
		Stat info(path);
		if (info.IsDirectory()) {
			// No empty string because the path has '/'
			if (path.back() != '/') {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::MOVED_PERMANENTLY),
					reason_phrase.at(http::MOVED_PERMANENTLY)
				);
				return;
			}
			// todo: Check for index directive and handle ReadFile function
			// todo: Check for autoindex directive and handle AutoindexHandler function
			// todo: Return 403 Forbidden if neither index nor autoindex directives exist
		} else if (info.IsRegularFile()) {
			if (!info.IsReadableFile()) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
				);
			} else {
				response_body_message = ReadFile(path);
			}
		} else {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::NOT_FOUND), reason_phrase.at(http::NOT_FOUND)
			);
		}
	} catch (const utils::SystemException &e) {
		HandleSystemException(e, response_body_message);
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
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (info.IsRegularFile()) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::NO_CONTENT), reason_phrase.at(http::NO_CONTENT)
			);
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
			HandleSystemException(e, response_body_message);
		}
	}
}

void HttpResponse::DeleteHandler(const std::string &path, std::string &response_body_message) {
	if (unlink(path.c_str()) == 0) {
		response_body_message = CreateDefaultBodyMessageFormat(
			utils::ToString(http::NO_CONTENT), reason_phrase.at(http::NO_CONTENT)
		);
	} else {
		if (errno == EACCES || errno == EISDIR || errno == EPERM) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (errno == ENOENT || errno == ENOTDIR || errno == ENAMETOOLONG) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::NOT_FOUND), reason_phrase.at(http::NOT_FOUND)
			);
		} else {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::INTERNAL_SERVER_ERROR),
				reason_phrase.at(http::INTERNAL_SERVER_ERROR)
			);
		}
	}
}

void HttpResponse::HandleSystemException(
	const utils::SystemException &e, std::string &response_body_message
) {
	int error_number = e.GetErrorNumber();
	if (error_number == EACCES) {
		response_body_message = CreateDefaultBodyMessageFormat(
			utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
		);
	} else if (error_number == ENOENT || error_number == ENOTDIR || error_number == ELOOP || error_number == ENAMETOOLONG) {
		response_body_message = CreateDefaultBodyMessageFormat(
			utils::ToString(http::NOT_FOUND), reason_phrase.at(http::NOT_FOUND)
		);
	} else {
		response_body_message = CreateDefaultBodyMessageFormat(
			utils::ToString(http::INTERNAL_SERVER_ERROR),
			reason_phrase.at(http::INTERNAL_SERVER_ERROR)
		);
	}
}

void HttpResponse::FileCreationHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message
) {
	std::ofstream file(path.c_str(), std::ios::binary);
	if (file.fail()) {
		response_body_message = CreateDefaultBodyMessageFormat(
			utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
		);
		return;
	}
	file.write(request_body_message.c_str(), request_body_message.length());
	if (file.fail()) {
		response_body_message = CreateDefaultBodyMessageFormat(
			utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
		);
		return;
	}
	response_body_message = CreateDefaultBodyMessageFormat(
		utils::ToString(http::CREATED), reason_phrase.at(http::CREATED)
	);
}

} // namespace http
