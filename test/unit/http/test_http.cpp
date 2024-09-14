#include "client_infos.hpp"
#include "http.hpp"
#include "http_result.hpp"
#include <cstdlib>

/**
 * @brief Test Http class: HttpResultの返り値を確認するテスト。
 *
 * テスト一覧（今後追加）
 * - メソッドが機能しているかどうか
 * 	- GET
 * 	- POST
 * 	- DELETE
 * - リクエストターゲットが機能しているかどうか(CGIは今後)
 * 	- 存在してるファイル
 * 	- 存在してないファイル
 * 	- 存在してるディレクトリ（末尾に/ありとなし）
 * 	- 存在してないディレクトリ（末尾に/ありとなし）
 * - ヘッダーフィールドごとに機能しているかどうか(今後、追加)
 * 	- Host
 * 	- Content-Length
 * - VirtualServer, Locationの各ディレクティブが機能しているかどうか
 * 	- VirtualServer
 * 	 - server_names
 * 	- Location
 * 	 - allow_methods
 *
 * responseの期待値はstatus_line, header_fields, CRLF, body_messageを組み合わせる。
 * status_lineとデフォルトのステータスコードを返すbody_messageはファイルで用意する。header_fieldsは可変のため文字列で作成する。
 * response以外のHttpResultのパラメータの期待値はテストごとに作成。
 *
 * HttpResult
 * - is_response_complete レスポンスの書式が完全かどうか
 * - is_connection_keep   次の接続が可能かどうか
 * - request_buf          バッファ
 * - response             文字列
 *
 * Locationのディレクティブを網羅するテストができるようなvirtual_serversを設定する。
 * client_infosはfdは1で固定する。request_bufはファイルから読み込むで文字列にする。
 *
 * @param client_infos
 * @param virtual_servers
 *
 * @return HttpResult indicating whether the response is complete
 *         and containing the response data.
 */

namespace {

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

int HandleResult(const Result &result) {
	if (result.is_success) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << result.error_log;
		return EXIT_FAILURE;
	}
}

http::MockDtoClientInfos
CreateMockDtoClientInfos(int fd, const std::string &request_buffer, const std::string &ip) {
	http::MockDtoClientInfos client_infos;
	client_infos.fd          = fd;
	client_infos.request_buf = request_buffer;
	client_infos.ip          = ip;
	return client_infos;
}

http::HttpResult CreateHttpResult(
	bool               is_response_complete,
	bool               is_connection_keep,
	const std::string &request_buffer,
	const std::string &response
) {
	http::HttpResult result;
	result.is_response_complete = is_response_complete;
	result.is_connection_keep   = is_connection_keep;
	result.request_buf          = request_buffer;
	result.response             = response;
	return result;
}

Result IsSameIsResponseComplete(bool is_response_complete, bool expected) {
	Result is_response_complete_result;
	if (is_response_complete != expected) {
		std::ostringstream error_log;
		error_log << "Error: Is Response Complete\n";
		error_log << "- Expected: [" << std::boolalpha << expected << "]\n";
		error_log << "- Result  : [" << std::boolalpha << is_response_complete << "]\n";
		is_response_complete_result.is_success = false;
		is_response_complete_result.error_log  = error_log.str();
	}
	return is_response_complete_result;
}

Result IsSameIsConnectionKeep(bool is_connection_keep, bool expected) {
	Result is_connection_keep_result;
	if (is_connection_keep != expected) {
		std::ostringstream error_log;
		error_log << "Error: Is Connection Keep\n";
		error_log << "- Expected: [" << std::boolalpha << expected << "]\n";
		error_log << "- Result  : [" << std::boolalpha << is_connection_keep << "]\n";
		is_connection_keep_result.is_success = false;
		is_connection_keep_result.error_log  = error_log.str();
	}
	return is_connection_keep_result;
}

Result IsSameRequestBuffer(const std::string &request_buffer, const std::string &expected) {
	Result request_buffer_result;
	if (request_buffer != expected) {
		std::ostringstream error_log;
		error_log << "Error: Request Buffer\n";
		error_log << "- Expected: [" << expected << "]\n";
		error_log << "- Result  : [" << request_buffer << "]\n";
		request_buffer_result.is_success = false;
		request_buffer_result.error_log  = error_log.str();
	}
	return request_buffer_result;
}

Result IsSameHttpResponse(const std::string &response, const std::string &expected) {
	Result http_response_result;
	if (response != expected) {
		std::ostringstream error_log;
		error_log << "Error: Http Response\n";
		error_log << "- Expected: [" << expected << "]\n";
		error_log << "- Result  : [" << response << "]\n";
		http_response_result.is_success = false;
		http_response_result.error_log  = error_log.str();
	}
	return http_response_result;
}

Result IsSameHttpResult(const http::HttpResult &http_result, const http::HttpResult expected) {
	Result result;
	result =
		IsSameIsResponseComplete(http_result.is_response_complete, expected.is_response_complete);
	if (!result.is_success) {
		return result;
	}
	result = IsSameIsConnectionKeep(http_result.is_connection_keep, expected.is_connection_keep);
	if (!result.is_success) {
		return result;
	}
	result = IsSameRequestBuffer(http_result.request_buf, expected.request_buf);
	if (!result.is_success) {
		return result;
	}
	result = IsSameHttpResponse(http_result.response, expected.response);
	return result;
}

int HandleHttpResult(
	const http::MockDtoClientInfos      &client_infos,
	const server::VirtualServerAddrList &server_infos,
	const http::HttpResult               expected
) {
	http::HttpResult http_result;
	(void)client_infos;
	(void)server_infos;
	// todo: ServerInfosを作成したら動く;
	// http::Http       http;
	// http::HttpResult http_result = http.Run(client_infos, server_infos);
	const Result &result = IsSameHttpResult(http_result, expected);
	return HandleResult(result);
}
} // namespace

int main(void) {
	int ret_code = EXIT_SUCCESS;

	// CreateHttpResult
	// input parameter in HttpResult
	// ouput HttpResult
	const std::string              &request_buffer = "ok";
	const http::MockDtoClientInfos &client_infos =
		CreateMockDtoClientInfos(1, request_buffer, "127.0.0.1");
	// todo: InitServerInfos;
	server::VirtualServerAddrList server_infos;
	http::HttpResult              expected = CreateHttpResult(false, true, "", "");
	ret_code |= HandleHttpResult(client_infos, server_infos, expected);

	return ret_code;
}
