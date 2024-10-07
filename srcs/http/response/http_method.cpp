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

namespace http {
namespace {

bool IsExistPath(const std::string &path) {
	return access(path.c_str(), F_OK) == 0;
}

std::string FileToString(const std::ifstream &file) {
	std::stringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

bool EndWith(const std::string &str, const std::string &suffix) {
	return str.size() >= suffix.size() &&
		   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

// ファイルの拡張子に基づいてContent-Typeを決定する関数: デフォルトはtext/plain
std::string DetermineContentType(const std::string &path) {
	const std::string html_extension = ".html";
	const std::string json_extension = ".json";
	const std::string pdf_extension  = ".pdf";

	if (EndWith(path, html_extension)) {
		return http::TEXT_HTML;
	} else if (EndWith(path, json_extension)) {
		return "application/json";
	} else if (EndWith(path, pdf_extension)) {
		return "application/pdf";
	}
	return TEXT_PLAIN;
}

// todo: utilsへ移動
// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
bool StartWith(const std::string &str, const std::string &prefix) {
	return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

// ヘルパー関数: 文字列のトリム: TrimOWSにも使える
std::string Trim(const std::string &str, const std::string &to_trim) {
	std::size_t first = str.find_first_not_of(to_trim);
	if (std::string::npos == first) {
		return "";
	}
	std::size_t last = str.find_last_not_of(to_trim);
	return str.substr(first, last - first + 1);
}

// std::string::front()
char FrontChar(const std::string &str) {
	return str.empty() ? '\0' : str[0];
}

// std::string::back()
char BackChar(const std::string &str) {
	return str.empty() ? '\0' : str[str.size() - 1];
}

// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// ヘルパー関数: 文字列のクオートを削除
std::string RemoveQuotes(const std::string &str) {
	if (FrontChar(str) == '"' && BackChar(str) == '"') {
		return str.substr(1, str.size() - 2);
	}
	return str;
}

} // namespace

StatusCode Method::Handler(
	const std::string  &path,
	const std::string  &method,
	const AllowMethods &allow_methods,
	const std::string  &request_body_message,
	const HeaderFields &request_header_fields,
	std::string        &response_body_message,
	HeaderFields       &response_header_fields,
	const std::string  &index_file_path,
	bool                autoindex_on,
	const std::string  &upload_directory
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
		status_code = PostHandler(
			path,
			request_body_message,
			request_header_fields,
			response_body_message,
			response_header_fields,
			upload_directory
		);
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
			response_header_fields[CONTENT_TYPE] = DetermineContentType(path + index_file_path);
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
			response_header_fields[CONTENT_TYPE] = DetermineContentType(path);
		}
	} else {
		throw HttpException("Error: Not Found", StatusCode(NOT_FOUND));
	}
	return status_code;
}

StatusCode Method::PostHandler(
	const std::string  &path,
	const std::string  &request_body_message,
	const HeaderFields &request_header_fields,
	std::string        &response_body_message,
	HeaderFields       &response_header_fields,
	const std::string  &upload_directory
) {
	// ex. test.txt
	const std::string file_name = path.substr(path.find_last_of('/') + 1);
	// ex. srcs/http/response/http_serverinfo_check/../../../../root
	const std::string root_path = path.substr(0, path.find("root/") + 4);
	// ex. srcs/http/response/http_serverinfo_check/../../../../root/save/test.txt
	const std::string upload_dir_path  = root_path + upload_directory;
	const std::string upload_file_path = upload_dir_path + "/" + file_name;

	if (upload_directory.empty()) {
		return EchoPostHandler(request_body_message, response_body_message, response_header_fields);
	} else if (request_header_fields.find(CONTENT_TYPE) != request_header_fields.end() &&
			   StartWith(request_header_fields.at(CONTENT_TYPE), MULTIPART_FORM_DATA)) {
		// Content-Type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW
		// のようにContent-Typeがmultipart/form-dataの場合
		return FileCreationHandlerForMultiPart(
			upload_dir_path,
			request_body_message,
			request_header_fields,
			response_body_message,
			response_header_fields
		);
	} else if (!IsExistPath(upload_file_path)) {
		return FileCreationHandler(
			upload_file_path, request_body_message, response_body_message, response_header_fields
		);
	}
	const Stat &info = TryStat(upload_file_path);
	StatusCode  status_code(NO_CONTENT);
	if (info.IsDirectory()) {
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
	} else if (info.IsRegularFile()) {
		response_body_message = HttpResponse::CreateDefaultBodyMessage(status_code);
		response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	} else {
		// - upload_file_pathがシンボリックリンク
		// - upload_file_pathが特殊ファイル（デバイスファイル、ソケットファイルなど）
		throw HttpException("Error: Forbidden", StatusCode(FORBIDDEN));
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

StatusCode Method::FileCreationHandlerForMultiPart(
	const std::string  &path,
	const std::string  &request_body_message,
	const HeaderFields &request_header_fields,
	std::string        &response_body_message,
	HeaderFields       &response_header_fields
) {
	std::vector<Method::Part> parts =
		DecodeMultipartFormData(request_header_fields.at(CONTENT_TYPE), request_body_message);
	for (std::vector<Method::Part>::iterator it = parts.begin(); it != parts.end(); ++it) {
		// Content-Disposition: form-data; name="file"; filename="test.txt"
		// のようにfilenameが含まれる場合 そのパスにファイルを作成する
		if ((*it).headers.find(CONTENT_DISPOSITION) == (*it).headers.end()) {
			continue;
		}
		std::map<std::string, std::string> content_disposition =
			ParseContentDisposition((*it).headers[CONTENT_DISPOSITION]);
		if (content_disposition.find(FILENAME) == content_disposition.end()) {
			continue;
		}
		std::string file_name = content_disposition[FILENAME];
		std::string file_path = path + "/" + file_name;
		// upload_dir + "/" + file_name にファイルを作成
		FileCreationHandler(file_path, (*it).body, response_body_message, response_header_fields);
	}
	StatusCode status_code(CREATED);
	response_body_message                  = HttpResponse::CreateDefaultBodyMessage(status_code);
	response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	return status_code;
}

StatusCode Method::FileCreationHandler(
	const std::string &path,
	const std::string &request_body_message,
	std::string       &response_body_message,
	HeaderFields      &response_header_fields
) {
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
	StatusCode status_code(CREATED);
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

std::string Method::ReadFile(const std::string &file_path) {
	std::ifstream file(file_path.c_str());
	if (!file) {
		SystemExceptionHandler(errno);
	}
	return FileToString(file);
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
	std::size_t       pos          = path.find(root_path);
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
			std::size_t padding = (entry_name.length() < 50) ? 50 - entry_name.length() : 0;
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

StatusCode Method::EchoPostHandler(
	const std::string &request_body_message,
	std::string       &response_body_message,
	HeaderFields      &response_header_fields
) {
	response_body_message                  = request_body_message;
	response_header_fields[CONTENT_LENGTH] = utils::ToString(response_body_message.length());
	return StatusCode(OK);
}

// multipart/form-dataをデコードする関数
std::vector<Method::Part>
Method::DecodeMultipartFormData(const std::string &content_type, const std::string &body) {
	std::vector<Method::Part> parts;
	std::string               boundary  = ExtractBoundary(content_type);
	std::vector<std::string>  raw_parts = utils::SplitStr(body, boundary);
	if (raw_parts.size() < 3) { // boundary + パート + boundary が最低でも必要
		throw HttpException("Error: Invalid multipart/form-data format", StatusCode(BAD_REQUEST));
	}
	for (std::vector<std::string>::const_iterator raw_part =
			 raw_parts.begin() + 1;       // 最初のパートは空文字列
		 raw_part != raw_parts.end() - 1; // 最後のパートはboundary + "--"
		 ++raw_part) {
		parts.push_back(ParsePart(*raw_part));
	}
	if (raw_parts.back() != "--\r\n") {
		// 最後はboundary + "--"(----WebKitFormBoundary7MA4YWxkTrZu0gW--)
		throw HttpException(
			"Error: Invalid multipart/form-data format, final boundary not found",
			StatusCode(BAD_REQUEST)
		);
	}
	return parts;
}

// Boundaryを抽出する関数
std::string Method::ExtractBoundary(const std::string &content_type) {
	const std::string boundary_prefix = BOUNDARY + "=";
	std::size_t       pos             = content_type.find(boundary_prefix);
	// Content-Type: multipart/form-data; boundary=--WebKitFormBoundary7MA4YWxkTrZu0gW
	if (pos != std::string::npos) {
		// ----WebKitFormBoundary7MA4YWxkTrZu0gW\r\n のような形式になっている
		return "--" + content_type.substr(pos + boundary_prefix.length());
	}
	throw HttpException(
		"Error: Boundary not found in Content-Type header", StatusCode(BAD_REQUEST)
	);
}

// ヘッダーとボディを分割する関数
Method::Part Method::ParsePart(const std::string &part) {
	Part        result;
	std::size_t header_end = part.find(HEADER_FIELDS_END);
	if (header_end == std::string::npos) {
		throw HttpException(
			"Error: Invalid part format, headers and body not properly separated",
			StatusCode(BAD_REQUEST)
		);
	}
	// ----WebKitFormBoundary7MA4YWxkTrZu0gW\r\nContent-Disposition: form-data; name="file";
	// のboundary=----WebKitFormBoundary7MA4YWxkTrZu0gW以降の\r\nから始まる
	std::string headers = part.substr(CRLF.length(), header_end);
	result.body         = part.substr(header_end + CRLF.length() * 2);

	std::size_t pos = 0;
	std::size_t end = headers.find(CRLF, pos);
	while (end != std::string::npos) {
		std::string              header            = headers.substr(pos, end - pos);
		std::vector<std::string> header_name_value = utils::SplitStr(header, ": ");
		if (header_name_value.size() != 2) {
			throw HttpException("Error: Invalid header format", StatusCode(BAD_REQUEST));
		}
		result.headers[header_name_value[0]] = header_name_value[1];
		pos                                  = end + CRLF.length();
		end                                  = headers.find(CRLF, pos);
	}
	return result;
}

// Content-Disposition ヘッダーをパースする関数
std::map<std::string, std::string> Method::ParseContentDisposition(const std::string &header) {
	std::map<std::string, std::string> result;
	std::istringstream                 stream(header);
	std::string                        part;

	// form-data; name="file"; filename="a.txt"
	std::getline(stream, part, ';'); // form-data
	// セミコロンで分割
	while (std::getline(stream, part, ';')) {
		part            = Trim(part, OPTIONAL_WHITESPACE);
		std::size_t pos = part.find('=');
		if (pos == std::string::npos) {
			throw HttpException(
				"Error: Invalid Content-Disposition header format", StatusCode(BAD_REQUEST)
			);
		}
		// filename="a.txt"のような形で来る
		std::string key   = Trim(part.substr(0, pos), OPTIONAL_WHITESPACE);
		std::string value = Trim(part.substr(pos + 1), OPTIONAL_WHITESPACE);
		value             = RemoveQuotes(value);
		result[key]       = value;
	}
	return result;
}

} // namespace http
