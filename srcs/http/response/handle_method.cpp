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
		std::ifstream error_file("html/404.html");
		utils::Debug("http", "404 file not found");
		return FileToString(error_file);
	}
	return FileToString(file);
}

bool IsWritablePath(const std::string &path) {
	return access(path.c_str(), W_OK) == 0;
}

std::string GetParentDirectoryPathName(const std::string &path) {
	std::size_t pos = path.find_last_of("/");
	if (pos == std::string::npos) {
		return ".";
	}
	return path.substr(0, pos);
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
			if (path[path.size() - 1] != '/') {
				// todo: return stats_code pathname;
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::MOVED_PERMANENTLY),
					reason_phrase.at(http::MOVED_PERMANENTLY)
				);
			}
			// todo: if index directive exists, it is called ReadFile function.
			// todo: if autoindex directive is on, it is called AutoindexHandler function.
			// todo: if index and autoindex directive don't exist, it is created 403 forbidden.
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
		int error_number = e.GetErrorNumber();
		if (error_number == EACCES) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (error_number == ENOENT || error_number == ENOTDIR) {
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
			std::ofstream file(path.c_str(), std::ios::binary);
			if (file.fail()) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::INTERNAL_SERVER_ERROR),
					reason_phrase.at(http::INTERNAL_SERVER_ERROR)
				);
				return;
			}
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::CREATED), reason_phrase.at(http::CREATED)
			);
			file.write(request_body_message.c_str(), request_body_message.length());
			if (file.fail()) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::INTERNAL_SERVER_ERROR),
					reason_phrase.at(http::INTERNAL_SERVER_ERROR)
				);
			}
		}
	} catch (const utils::SystemException &e) {
		int error_number = e.GetErrorNumber();
		if (error_number == EACCES) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (error_number == ENOENT || error_number == ENOTDIR) {
			// todo: CreateFile();
			std::ofstream file(path.c_str(), std::ios::binary);
			if (file.fail()) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::INTERNAL_SERVER_ERROR),
					reason_phrase.at(http::INTERNAL_SERVER_ERROR)
				);
				return;
			}
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::CREATED), reason_phrase.at(http::CREATED)
			);
			file.write(request_body_message.c_str(), request_body_message.length());
			if (file.fail()) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::INTERNAL_SERVER_ERROR),
					reason_phrase.at(http::INTERNAL_SERVER_ERROR)
				);
			}
		} else {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::INTERNAL_SERVER_ERROR),
				reason_phrase.at(http::INTERNAL_SERVER_ERROR)
			);
		}
	}
}

// todo: ExecuteDelete: リソースの削除
// 成功した場合、204 No Content
// -> 詳細を表示しない設定にするため
// パスがディレクトリの場合(autoindexはon,
// off関係なし): 403 Forbidden
// ファイル権限がない場合: 403 Forbidden
// 存在しないファイルの場合: 404 Not Found
void HttpResponse::DeleteHandler(const std::string &path, std::string &response_body_message) {
	try {
		Stat info(path);
		if (info.IsDirectory()) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (info.IsRegularFile()) {
			if (!IsWritablePath(GetParentDirectoryPathName(path))) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
				);
				return;
			}
			if (std::remove("ok.txt") == 0) {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::NO_CONTENT), reason_phrase.at(http::NO_CONTENT)
				);
			} else {
				response_body_message = CreateDefaultBodyMessageFormat(
					utils::ToString(http::INTERNAL_SERVER_ERROR),
					reason_phrase.at(http::INTERNAL_SERVER_ERROR)
				);
			}
		} else {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::NOT_FOUND), reason_phrase.at(http::NOT_FOUND)
			);
		}
	} catch (const utils::SystemException &e) {
		int error_number = e.GetErrorNumber();
		if (error_number == EACCES) {
			response_body_message = CreateDefaultBodyMessageFormat(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (error_number == ENOENT || error_number == ENOTDIR) {
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

} // namespace http
