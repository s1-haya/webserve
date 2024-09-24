#include "cgi_manager.hpp"
#include "http_exception.hpp" // todo: remove
#include "http_message.hpp"
#include "status_code.hpp" // todo: remove
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace {

using namespace cgi;

const std::string PATH_DIR_CGI_BIN = "../../../../root/cgi-bin";

typedef server::CgiManager      CgiManager;
typedef cgi::Cgi::CgiResult     CgiResult;
typedef CgiManager::GetFdResult GetFdResult;

struct Result {
	Result() : is_success(true) {}
	bool        is_success;
	std::string error_log;
};

int GetTestCaseNum() {
	static int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

void PrintError(const std::string &message) {
	std::cerr << utils::color::RED << message << utils::color::RESET << std::endl;
}

template <typename T>
bool IsSame(const T &result, const T &expected) {
	return result == expected;
}

int Test(Result result) {
	if (result.is_success) {
		PrintOk();
		return EXIT_SUCCESS;
	}
	PrintNg();
	PrintError(result.error_log);
	return EXIT_FAILURE;
}

// test_funcを実行したらthrowされることを期待する関数
template <typename TestFunc>
int TestThrow(TestFunc test_func, const CgiManager &cgi_manager, int client_fd) {
	try {
		(cgi_manager.*test_func)(client_fd);
		PrintNg();
		PrintError("throw failed");
		return EXIT_FAILURE;
	} catch (const std::exception &e) {
		PrintOk();
		std::cerr << utils::color::GRAY << e.what() << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	}
}

// -----------------------------------------------------------------------------
Result
RunGetRequest(const CgiManager &cgi_manager, int client_fd, const std::string &expected_request) {
	Result             result;
	std::ostringstream oss;

	const std::string &cgi_request = cgi_manager.GetRequest(client_fd);
	if (!IsSame(cgi_request, expected_request)) {
		result.is_success = false;
		oss << "request" << std::endl;
		oss << "- result  : " << cgi_request << std::endl;
		oss << "- expected: " << expected_request << std::endl;
		result.error_log = oss.str();
	}
	return result;
}

Result
RunGetResponse(const CgiManager &cgi_manager, int client_fd, const std::string &expected_response) {
	Result             result;
	std::ostringstream oss;

	const std::string &cgi_response = cgi_manager.GetResponse(client_fd);
	if (!IsSame(cgi_response, expected_response)) {
		result.is_success = false;
		oss << "response" << std::endl;
		oss << "- result  : " << cgi_response << std::endl;
		oss << "- expected: " << expected_response << std::endl;
		result.error_log = oss.str();
	}
	return result;
}

Result IsSameClientFd(const CgiManager &cgi_manager, int pipe_fd, int expected_client_fd) {
	Result             result;
	std::ostringstream oss;

	const int result_client_fd = cgi_manager.GetClientFd(pipe_fd);
	if (!IsSame(result_client_fd, expected_client_fd)) {
		result.is_success = false;
		oss << "client_fd" << std::endl;
		oss << "- result  : " << result_client_fd << std::endl;
		oss << "- expected: " << expected_client_fd << std::endl;
		result.error_log = oss.str();
	}
	return result;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - AddNewCgi()
// - GetRequest()
// - ReplaceNewRequest()
// -----------------------------------------------------------------------------
int RunTest1() {
	int ret_code = EXIT_SUCCESS;

	const int         client_fd        = 3;
	const std::string expected_request = "abcde";

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";
	cgi_request.body_message                   = expected_request;

	// CgiManager内で新規Cgi作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(client_fd, cgi_request);

	// getterを使ってrequestの保持確認
	ret_code |= Test(RunGetRequest(cgi_manager, client_fd, expected_request));

	// "abc"だけwrite()できたと仮定し,残りのrequest("de")で置き換え
	const std::string expected_new_request = "de";
	cgi_manager.ReplaceNewRequest(client_fd, expected_new_request);
	// 置き換えできてるか確認
	ret_code |= Test(RunGetRequest(cgi_manager, client_fd, expected_new_request));

	return ret_code;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - DeleteCgi()
// -----------------------------------------------------------------------------
int RunTest2() {
	int ret_code = EXIT_SUCCESS;

	const int client_fd = 3;

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";

	// CgiManager内で新規Cgi作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(client_fd, cgi_request);

	// cgiを削除
	cgi_manager.DeleteCgi(client_fd);
	// 存在しないcgiにアクセスしようとしてthrowされることを確認
	ret_code |= TestThrow(&CgiManager::GetRequest, cgi_manager, client_fd);

	return ret_code;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - AddReadBuf()
// - GetResponse()
// -----------------------------------------------------------------------------
int RunTest3() {
	int ret_code = EXIT_SUCCESS;

	const int client_fd = 3;

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";

	// CgiManager内で新規Cgi作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(client_fd, cgi_request);

	// "abc"をread()できたとしてresponseに追加
	std::string expected_response = "abc";
	cgi_manager.AddReadBuf(client_fd, expected_response);

	// getterを使ってresponseの保持確認
	ret_code |= Test(RunGetResponse(cgi_manager, client_fd, expected_response));

	// 追加で"defgh"をread()できたとして,responseが"abc"+"defgh"になってるか確認
	const std::string appended_response = "defgh";
	expected_response += appended_response;
	cgi_manager.AddReadBuf(client_fd, appended_response);
	// responseに足せてるか確認
	ret_code |= Test(RunGetResponse(cgi_manager, client_fd, expected_response));

	return ret_code;
}

// todo: cgi.Run()が動くまではif文に入らずTest()が呼ばれてない
// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - RunCgi()
// - GetClientFd()
// - GetReadFd()
// - GetWriteFd()
// -----------------------------------------------------------------------------
int RunTest4() {
	int ret_code = EXIT_SUCCESS;

	const int         expected_client_fd = 3;
	const std::string expected_request   = "abcde";

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";
	cgi_request.body_message                   = expected_request;

	// CgiManager内で新規Cgiを複数作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(expected_client_fd, cgi_request);
	cgi_manager.AddNewCgi(4, cgi_request);

	// cgiを実行する(子プロセスで実行 & pipe_fdがclient_fdと紐づけられる)
	const CgiResult run_cgi_result = cgi_manager.RunCgi(expected_client_fd);

	// pipe_fd(read)を取得
	const GetFdResult read_fd_result = cgi_manager.GetReadFd(expected_client_fd);
	if (read_fd_result.IsOk()) {
		const int read_pipe_fd = read_fd_result.GetValue();
		// read_pipe_fdとclient_fdの紐づけ確認
		ret_code |= Test(IsSameClientFd(cgi_manager, read_pipe_fd, expected_client_fd));
	}

	// pipe_fd(write)を取得
	const GetFdResult write_fd_result = cgi_manager.GetWriteFd(expected_client_fd);
	if (write_fd_result.IsOk()) {
		const int write_pipe_fd = write_fd_result.GetValue();
		// write_pipe_fdとclient_fdの紐づけ確認
		ret_code |= Test(IsSameClientFd(cgi_manager, write_pipe_fd, expected_client_fd));
	}

	return ret_code;
}

// todo: Cgi classの実装が終わったら作成
// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - IsResponseComplete()
// -----------------------------------------------------------------------------
// int RunTest5() {
// 	int ret_code = EXIT_SUCCESS;
// 	return ret_code;
// }

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTest1();
	ret_code |= RunTest2();
	ret_code |= RunTest3();
	ret_code |= RunTest4();
	// ret_code |= RunTest5();

	return ret_code;
}
