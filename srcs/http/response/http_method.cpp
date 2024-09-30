#include "http_method.hpp"
#include "http_exception.hpp"
#include "http_message.hpp"
#include "http_method.hpp"
#include "http_response.hpp"
#include "http_serverinfo_check.hpp"
#include "stat.hpp"
#include <algorithm> // std::find
#include <cstring>
#include <ctime>    // ctime
#include <dirent.h> // opendir, readdir, closedir
#include <fstream>
#include <iostream>
#include <sstream>
#include <sys/stat.h>
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
		std::ifstream error_file("root/html/404.html");
		return FileToString(error_file);
	}
	return FileToString(file);
}

// ファイルの拡張子に基づいてContent-Typeを決定する関数: デフォルトはtext/plain
std::string determineContentType(const std::string &path) {
	const std::string html_extension = ".html";
	if (path.size() >= html_extension.length() &&
		path.compare(
			path.size() - html_extension.length(), html_extension.length(), html_extension
		) == 0) {
		return "text/html";
	}
	return "text/plain";
}

} // namespace

namespace http {

StatusCode Method::Handler(
	const std::string  &path,
	const std::string  &method,
	const AllowMethods &allow_methods,
	const std::string  &request_body_message,
	std::string        &response_body_message,
	HeaderFields       &response_header_fields,
	const std::string  &index_file_path,
	bool                autoindex_on
) {
	StatusCode status_code(OK);
	if (!IsSupportedMethod(method)) {
		throw HttpException("Error: Not Implemented", StatusCode(NOT_IMPLEMENTED));
	}
	if (!IsAllowedMethod(method, allow_methods)) {
		throw HttpException("Error: Method Not Allowed", StatusCode(METHOD_NOT_ALLOWED));
	}
	if (method == GET) {
		status_code = GetHandler(
			path, response_body_message, response_header_fields, index_file_path, autoindex_on
		);
	} else if (method == POST) {
		status_code =
			PostHandler(path, request_body_message, response_body_message, response_header_fields);
	} else if (method == DELETE) {
		status_code = DeleteHandler(path, response_body_message, response_header_fields);
	}
	return status_code;
}

StatusCode Method::GetHandler(
	const std::string &path,
	std::string       &response_body_message,
	HeaderFields      &response_header_fields,
	const std::string &index_file_path,
	bool               autoindex_on
) {
	StatusCode  status_code(OK);
	const Stat &info = TryStat(path);
	if (info.IsDirectory()) {
		// No empty string because the path has '/'
		if (path[path.size() - 1] != '/') {
			throw HttpException("Error: Moved Permanently", StatusCode(MOVED_PERMANENTLY));
		} else if (!index_file_path.empty()) {
			response_body_message = ReadFile(path + index_file_path);
			response_header_fields[CONTENT_LENGTH] =
				utils::ToString(response_body_message.length());
			response_header_fields[CONTENT_TYPE] = determineContentType(path + index_file_path);
		} else if (autoindex_on) {
			utils::Result<std::string> result = AutoindexHandler(path);
			response_body_message             = result.GetValue();
			response_header_fields[CONTENT_LENGTH] =
				utils::ToString(response_body_message.length());
			if (!result.IsOk()) {
				throw HttpException(
					"Error: Internal Server Error", StatusCode(INTERNAL_SERVER_ERROR)
				);
			}
		} else {
			throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
		}
	} else if (info.IsRegularFile()) {
		if (!info.IsReadableFile()) {
			throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
		} else {
			response_body_message = ReadFile(path);
			response_header_fields[CONTENT_LENGTH] =
				utils::ToString(response_body_message.length());
			response_header_fields[CONTENT_TYPE] = determineContentType(path);
		}
	} else {
		throw HttpException("Error: Not Found", StatusCode(NOT_FOUND));
	}
	return status_code;
}

StatusCode Method::PostHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message,
	HeaderFields      &response_header_fields
) {
	if (!IsExistPath(path)) {
		return FileCreationHandler(
			path, request_body_message, response_body_message, response_header_fields
		);
	}
	const Stat &info = TryStat(path);
	StatusCode  status_code(NO_CONTENT);
	if (info.IsDirectory()) {
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
	} else if (info.IsRegularFile()) {
		response_body_message = HttpResponse::CreateDefaultBodyMessage(status_code);
		response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	} else {
		// Location header fields: URI-reference
		// ex) POST /save/test.txt HTTP/1.1
		// Location: /save/test.txt;
		status_code = FileCreationHandler(
			path, request_body_message, response_body_message, response_header_fields
		);
	}
	return status_code;
}

StatusCode Method::DeleteHandler(
	const std::string &path,
	std::string       &response_body_message,
	HeaderFields      &response_header_fields
) {
	const Stat &info        = TryStat(path);
	StatusCode  status_code = StatusCode(NO_CONTENT);
	if (info.IsDirectory()) {
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
	}
	if (std::remove(path.c_str()) == SYSTEM_ERROR) {
		SystemExceptionHandler(errno);
	} else {
		response_body_message = HttpResponse::CreateDefaultBodyMessage(status_code);
		response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	}
	return status_code;
}

void Method::SystemExceptionHandler(int error_number) {
	if (error_number == EACCES || error_number == EPERM) {
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
	} else if (error_number == ENOENT || error_number == ENOTDIR || error_number == ELOOP ||
			   error_number == ENAMETOOLONG) {
		throw HttpException("Error: Not Found", StatusCode(NOT_FOUND));
	} else {
		throw HttpException("Error: Internal Server Error", StatusCode(INTERNAL_SERVER_ERROR));
	}
}

StatusCode Method::FileCreationHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message,
	HeaderFields      &response_header_fields
) {
	StatusCode    status_code(CREATED);
	std::ofstream file(path.c_str(), std::ios::binary);
	if (file.fail()) {
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
	}
	file.write(request_body_message.c_str(), request_body_message.length());
	if (file.fail()) {
		file.close();
		if (std::remove(path.c_str()) == SYSTEM_ERROR) {
			SystemExceptionHandler(errno);
		}
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
	}
	response_body_message                  = HttpResponse::CreateDefaultBodyMessage(status_code);
	response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	return status_code;
}

Stat Method::TryStat(const std::string &path) {
	struct stat stat_buf;
	if (stat(path.c_str(), &stat_buf) == SYSTEM_ERROR) {
		SystemExceptionHandler(errno);
	}
	Stat info(stat_buf);
	return info;
}

bool Method::IsSupportedMethod(const std::string &method) {
	return std::find(DEFAULT_METHODS, DEFAULT_METHODS + DEFAULT_METHODS_SIZE, method) !=
		   DEFAULT_METHODS + DEFAULT_METHODS_SIZE;
}

bool Method::IsAllowedMethod(
	const std::string &method, const std::list<std::string> &allow_methods
) {
	if (allow_methods.empty()) {
		return std::find(
				   DEFAULT_ALLOWED_METHODS,
				   DEFAULT_ALLOWED_METHODS + DEFAULT_ALLOWED_METHODS_SIZE,
				   method
			   ) != DEFAULT_ALLOWED_METHODS + DEFAULT_ALLOWED_METHODS_SIZE;
	} else {
		return std::find(allow_methods.begin(), allow_methods.end(), method) != allow_methods.end();
	}
}

utils::Result<std::string> Method::AutoindexHandler(const std::string &path) {
	utils::Result<std::string> result;
	DIR                       *dir = opendir(path.c_str());
	std::string                response_body_message;

	if (dir == NULL) {
		result.Set(false);
		return result;
	}

	std::string       display_path = path;
	const std::string root_path    = "/root";
	size_t            pos          = path.find(root_path);
	if (pos != std::string::npos) {
		display_path = path.substr(pos + root_path.length());
	}

	struct dirent *entry;
	response_body_message += "<html>\n"
							 "<head><title>Index of " +
							 display_path +
							 "</title></head>\n"
							 "<body><h1>Index of " +
							 display_path +
							 "</h1><hr><pre>"
							 "<a href=\"../\">../</a>\n";

	errno = 0;
	while ((entry = readdir(dir)) != NULL) {
		if (entry->d_name[0] == '.') {
			continue;
		}
		std::string full_path = path + "/" + entry->d_name;
		struct stat file_stat;
		if (stat(full_path.c_str(), &file_stat) == 0) {
			bool        is_dir     = S_ISDIR(file_stat.st_mode);
			std::string entry_name = std::string(entry->d_name) + (is_dir ? "/" : "");
			// エントリ名の幅を固定
			response_body_message += "<a href=\"" + entry_name + "\">" + entry_name + "</a>";
			size_t padding = (entry_name.length() < 50) ? 50 - entry_name.length() : 0;
			response_body_message += std::string(padding, ' ') + " ";

			// ctimeの部分を固定幅にする
			char time_buf[20];
			std::strftime(
				time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M:%S", std::localtime(&file_stat.st_mtime)
			);
			response_body_message += std::string(time_buf) + " ";

			// bytesの部分を固定幅にする
			std::string size_str = is_dir ? "-" : utils::ToString(file_stat.st_size) + " bytes";
			padding              = (size_str.length() < 20) ? 20 - size_str.length() : 0;
			response_body_message += std::string(padding, ' ') + size_str + "\n";
		} else {
			result.Set(false);
		}
	}
	if (errno != 0) {
		result.Set(false);
	}

	response_body_message += "</pre><hr></body>\n</html>";
	closedir(dir);

	result.SetValue(response_body_message);
	return result;
}

} // namespace http
