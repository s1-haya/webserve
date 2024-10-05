#include "cgi_manager.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream>

namespace {

using namespace cgi;

const std::string PATH_DIR_CGI_BIN = "../../../../root/cgi-bin";

typedef server::CgiManager      CgiManager;
typedef CgiManager::GetFdResult GetFdResult;

struct Result {
	Result() : is_success(true) {}
	Result(bool is_success, const std::string &error_message)
		: is_success(is_success), error_log(error_message) {}

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

Result RunAddAndGetResponse(
	CgiManager        &cgi_manager,
	int                client_fd,
	const std::string &read_buf,
	const CgiResponse &expected_response
) {
	Result             result;
	std::ostringstream oss;

	const cgi::CgiResponse cgi_response = cgi_manager.AddAndGetResponse(client_fd, read_buf);
	if (!IsSame(cgi_response.response, expected_response.response) ||
		!IsSame(cgi_response.is_response_complete, expected_response.is_response_complete)) {
		result.is_success = false;
		oss << "response" << std::endl;
		oss << "- result(response)      : " << cgi_response.response << std::endl;
		oss << "- expected(response)    : " << expected_response.response << std::endl;
		oss << "- result(is_response_complete)  : " << cgi_response.is_response_complete
			<< std::endl;
		oss << "- expected(is_response_complete): " << expected_response.is_response_complete
			<< std::endl;
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

Result RunIsCgiExist(const CgiManager &cgi_manager, int fd, bool expected_result_is_cgi_exist) {
	Result             result;
	std::ostringstream oss;

	const bool result_is_cgi_exist = cgi_manager.IsCgiExist(fd);
	if (result_is_cgi_exist != expected_result_is_cgi_exist) {
		result.is_success = false;
		oss << "is_cgi_exist" << std::endl;
		oss << "- result  : " << std::boolalpha << result_is_cgi_exist << std::endl;
		oss << "- expected: " << expected_result_is_cgi_exist << std::endl;
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
	ret_code |= Test(RunGetRequest(cgi_manager, client_fd, expected_request)); // Test1

	// "abc"だけwrite()できたと仮定し,残りのrequest("de")で置き換え
	const std::string expected_new_request = "de";
	cgi_manager.ReplaceNewRequest(client_fd, expected_new_request);
	// 置き換えできてるか確認
	ret_code |= Test(RunGetRequest(cgi_manager, client_fd, expected_new_request)); // Test2

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
	ret_code |= TestThrow(&CgiManager::GetRequest, cgi_manager, client_fd); // Test3

	return ret_code;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - AddAndGetResponse()
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
	std::string buffer = "abc";
	// addしてgetterを使ってresponseの保持確認
	cgi::CgiResponse expected_cgi_response("abc", false);
	ret_code |=
		Test(RunAddAndGetResponse(cgi_manager, client_fd, buffer, expected_cgi_response)); // Test4

	// 追加で"defgh"をread()できたとして,responseが"abc"+"defgh"になってるか確認
	const std::string appended_buffer = "defgh";
	buffer += appended_buffer;
	ret_code |= Test(RunAddAndGetResponse(
		cgi_manager, client_fd, appended_buffer, cgi::CgiResponse(buffer, false)
	)); // Test5

	// 最後に""をread()したとして、responseが完成になっているか確認
	ret_code |=
		Test(RunAddAndGetResponse(cgi_manager, client_fd, "", cgi::CgiResponse(buffer, true))
		); // Test6

	return ret_code;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - RunCgi()
// - GetClientFd()
// - GetReadFd()
// - GetWriteFd()
// - method == GET
// -----------------------------------------------------------------------------
int RunTest4() {
	int ret_code = EXIT_SUCCESS;

	const int         expected_client_fd = 10; // 3,4はpipe_fdで使われる
	const std::string expected_request   = "abcde";

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";
	cgi_request.body_message                   = expected_request;

	// CgiManager内で新規Cgiを複数作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(expected_client_fd, cgi_request);
	cgi_manager.AddNewCgi(11, cgi_request);

	// cgiを実行する(子プロセスで実行 & pipe_fdがclient_fdと紐づけられる)
	cgi_manager.RunCgi(expected_client_fd);

	// pipe_fd(read)を取得
	const GetFdResult read_fd_result = cgi_manager.GetReadFd(expected_client_fd);
	if (read_fd_result.IsOk()) {
		const int read_pipe_fd = read_fd_result.GetValue();
		// read_pipe_fdとclient_fdの紐づけ確認
		ret_code |= Test(IsSameClientFd(cgi_manager, read_pipe_fd, expected_client_fd)); // Test7
	} else {
		ret_code |= Test(Result(false, "read pipe_fd was not created"));
	}

	// pipe_fd(write)を取得
	const GetFdResult write_fd_result = cgi_manager.GetWriteFd(expected_client_fd);
	if (write_fd_result.IsOk()) {
		ret_code |= Test(Result(false, "write pipe_fd was created")); // GETなのでここには入らない
	} else {
		ret_code |= Test(Result()); // Test8
	}

	return ret_code;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - RunCgi()
// - GetReadFd()
// - GetWriteFd()
// - method == POST
// -----------------------------------------------------------------------------
int RunTest5() {
	int ret_code = EXIT_SUCCESS;

	const int         expected_client_fd = 10;
	const std::string expected_request   = "abcde";

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "POST";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";
	cgi_request.body_message                   = expected_request;

	// CgiManager内で新規Cgiを作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(expected_client_fd, cgi_request);

	// cgiを実行する(子プロセスで実行 & pipe_fdがclient_fdと紐づけられる)
	cgi_manager.RunCgi(expected_client_fd);

	// pipe_fd(read)を取得
	const GetFdResult read_fd_result = cgi_manager.GetReadFd(expected_client_fd);
	if (read_fd_result.IsOk()) {
		const int read_pipe_fd = read_fd_result.GetValue();
		// read_pipe_fdとclient_fdの紐づけ確認
		ret_code |= Test(IsSameClientFd(cgi_manager, read_pipe_fd, expected_client_fd)); // Test9
	} else {
		ret_code |= Test(Result(false, "read pipe_fd was not created"));
	}

	// pipe_fd(write)を取得
	const GetFdResult write_fd_result = cgi_manager.GetWriteFd(expected_client_fd);
	if (write_fd_result.IsOk()) {
		const int write_pipe_fd = write_fd_result.GetValue();
		// write_pipe_fdとclient_fdの紐づけ確認
		ret_code |= Test(IsSameClientFd(cgi_manager, write_pipe_fd, expected_client_fd)); // Test10
	} else {
		ret_code |= Test(Result(false, "write pipe_fd was not created"));
	}

	return ret_code;
}

// -----------------------------------------------------------------------------
// CgiManager classの主なテスト対象関数
// - IsCgiExist()
// -----------------------------------------------------------------------------
int RunTest6() {
	int ret_code = EXIT_SUCCESS;

	const int         expected_client_fd1 = 10;
	const int         expected_client_fd2 = 11;
	const std::string expected_request    = "abcde";

	// 最低限のCgiRequest準備
	CgiRequest cgi_request;
	cgi_request.meta_variables[REQUEST_METHOD] = "GET";
	cgi_request.meta_variables[SCRIPT_NAME]    = PATH_DIR_CGI_BIN + "/test.sh";
	cgi_request.body_message                   = expected_request;

	// CgiManager内で新規Cgiを複数作成
	CgiManager cgi_manager;
	cgi_manager.AddNewCgi(expected_client_fd1, cgi_request);
	cgi_manager.AddNewCgi(expected_client_fd2, cgi_request);

	// cgiを実行する(子プロセスで実行 & pipe_fdがclient_fdと紐づけられる)
	cgi_manager.RunCgi(expected_client_fd1);

	// client_fdからIsCgiExist()==trueになるか確認
	ret_code |= Test(RunIsCgiExist(cgi_manager, expected_client_fd1, true)); // Test11
	ret_code |= Test(RunIsCgiExist(cgi_manager, expected_client_fd2, true)); // Test12

	// client_fd1のpipe_fd(read)を取得
	int               read_pipe_fd1  = -1;
	const GetFdResult read_fd_result = cgi_manager.GetReadFd(expected_client_fd1);
	if (read_fd_result.IsOk()) {
		read_pipe_fd1 = read_fd_result.GetValue();
		// read_pipe_fdからIsCgiExist()==trueになるか確認
		ret_code |= Test(RunIsCgiExist(cgi_manager, read_pipe_fd1, true)); // Test13
	} else {
		ret_code |=
			Test(Result(false, "read pipe_fd was not created")); // GETなのでここには入らない
	}

	// client_fd1のpipe_fd(write)を取得
	const GetFdResult write_fd_result = cgi_manager.GetWriteFd(expected_client_fd1);
	if (write_fd_result.IsOk()) {
		ret_code |= Test(Result(false, "write pipe_fd was created")); // GETなのでここには入らない
	} else {
		// write_pipe_fdは取得できないので適当な大きめfdでfalseが返ることを確認
		ret_code |= Test(RunIsCgiExist(cgi_manager, 20, false)); // Test14
	}

	// client_fd1を削除してみて
	cgi_manager.DeleteCgi(expected_client_fd1);

	// client_fd1だけIsCgiExist()==falseになるか確認
	ret_code |= Test(RunIsCgiExist(cgi_manager, expected_client_fd1, false)); // Test15
	ret_code |= Test(RunIsCgiExist(cgi_manager, expected_client_fd2, true));  // Test16

	// client_fd1のread_pipe_fd1からもIsCgiExist()==falseになるか確認
	ret_code |= Test(RunIsCgiExist(cgi_manager, read_pipe_fd1, false)); // Test17

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTest1();
	ret_code |= RunTest2();
	ret_code |= RunTest3();
	ret_code |= RunTest4();
	ret_code |= RunTest5();
	ret_code |= RunTest6();

	return ret_code;
}
