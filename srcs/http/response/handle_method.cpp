#include "http_response.hpp"
#include "stat.hpp"
#include "system_exception.hpp"
#include "utils.hpp"
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

// bool TryStat(const std::string& path, bool &is_success) {
// }

} // namespace

// todo: 各メソッドを実行する関数
// returnがある場合、設定する
// - メソッドの権限の確認
//  -> 権限がない場合405 Not Allowed
// - 各メソッドを実行

namespace http {

void HttpResponse::GetHandler(const std::string &path, std::string &body_message) {
	try {
		Stat info(path);
		if (info.IsDirectory()) {
			if (path[path.size() - 1] != '/') {
				// todo: return stats_code pathname;
				body_message = CreateErrorBodyMessage(
					utils::ToString(http::MOVED_PERMANENTLY),
					reason_phrase.at(http::MOVED_PERMANENTLY)
				);
			}
			// todo: if index directive exists, it is called ReadFile function.
			// todo: if autoindex directive is on, it is called AutoindexHandler function.
			// todo: if index and autoindex directive don't exist, it is created 403 forbidden.
		} else if (info.IsRegularFile()) {
			if (!info.IsReadableFile()) {
				body_message = CreateErrorBodyMessage(
					utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
				);
			} else {
				body_message = ReadFile(path);
			}
		} else {
			body_message = CreateErrorBodyMessage(
				utils::ToString(http::NOT_FOUND), reason_phrase.at(http::NOT_FOUND)
			);
		}
	} catch (const utils::SystemException &e) {
		int error_number = e.GetErrorNumber();
		if (error_number == EACCES) {
			body_message = CreateErrorBodyMessage(
				utils::ToString(http::FORBIDDEN), reason_phrase.at(http::FORBIDDEN)
			);
		} else if (error_number == ENOENT || error_number == ENOTDIR) {
			body_message = CreateErrorBodyMessage(
				utils::ToString(http::NOT_FOUND), reason_phrase.at(http::NOT_FOUND)
			);
		} else {
			body_message = CreateErrorBodyMessage(
				utils::ToString(http::INTERNAL_SERVER_ERROR),
				reason_phrase.at(http::INTERNAL_SERVER_ERROR)
			);
		}
	}
}

} // namespace http

// todo: ExecutePost: ボディメッセージをリソースに書き込む。ファイルアップロード
// パスがディレクトリの場合(autoindexはon, off関係なし): 403 Forbidden
// ファイル権限がない場合: 403 Forbidden
// 存在しないファイルの場合: 404 Not Found
// ファイルの書き込みが失敗した場合: 500 Internal Server Error

// todo: ExecuteDelete: リソースの削除
// 成功した場合、204 No Content
// -> 詳細を表示しない設定にするため
// パスがディレクトリの場合(autoindexはon,
// off関係なし): 403 Forbidden
// ファイル権限がない場合: 403 Forbidden
// 存在しないファイルの場合: 404 Not Found
