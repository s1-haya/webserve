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

typedef server::CgiManager CgiManager;

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

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTest1();

	return ret_code;
}
