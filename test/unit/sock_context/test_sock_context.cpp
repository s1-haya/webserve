#include "client_info.hpp"
#include "color.hpp"
#include "server_info.hpp"
#include "sock_context.hpp"
#include "utils.hpp"
#include <cstdlib>
#include <iostream>
#include <sstream> // ostringstream
#include <string>

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

void PrintOk() {
	std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK] " << utils::color::RESET
			  << std::endl;
}

void PrintNg() {
	std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
			  << std::endl;
}

void PrintError(const std::string &message) {
	std::cerr << utils::color::RED << message << utils::color::RESET << std::endl;
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
bool IsSameClientInfo(const server::ClientInfo &a, const server::ClientInfo &b) {
	return a.GetFd() == b.GetFd() && a.GetListenIp() == b.GetListenIp() &&
		   a.GetListenPort() == b.GetListenPort();
}

// ClientInfo同士のメンバが全て等しいことを期待するテスト
// (todo: 呼び出し側でClientInfoのメンバがセットできてないので今必ずtrueが返る)
Result RunGetClientInfo(
	const server::SockContext                &context,
	const server::SockContext::ClientInfoMap &expected_client_info,
	int                                       client_fd
) {
	// テスト対象のgetter
	const server::ClientInfo &a = context.GetClientInfo(client_fd);
	const server::ClientInfo &b = expected_client_info.at(client_fd);

	Result result;
	if (!IsSameClientInfo(a, b)) {
		result.is_success = false;
		std::ostringstream oss;
		oss << "client_fd  : result   [" << a.GetFd() << "]" << std::endl;
		oss << "             expected [" << b.GetFd() << "]" << std::endl;
		oss << "listen_IP  : result   [" << a.GetListenPort() << "]" << std::endl;
		oss << "             expected [" << b.GetListenPort() << "]" << std::endl;
		oss << "listen_port: result   [" << a.GetListenPort() << "]" << std::endl;
		oss << "             expected [" << b.GetListenPort() << "]" << std::endl;
		result.error_log = oss.str();
	}
	return result;
}

bool IsSameServerInfo(const server::ServerInfo &a, const server::ServerInfo &b) {
	return a.GetFd() == b.GetFd() && a.GetHost() == b.GetHost() && a.GetPort() == b.GetPort();
}

// ServerInfo同士のメンバが全て等しいことを期待するテスト
Result RunGetConnectedServerInfo(
	const server::SockContext                    &context,
	const server::SockContext::HostServerInfoMap &expected_host_server_info,
	int                                           client_fd
) {
	// テスト対象のgetter
	const server::ServerInfo &a = context.GetConnectedServerInfo(client_fd);
	const server::ServerInfo &b = *expected_host_server_info.at(client_fd);

	Result result;
	if (!IsSameServerInfo(a, b)) {
		result.is_success = false;
		std::ostringstream oss;
		oss << "server_fd  : result   [" << a.GetFd() << "]" << std::endl;
		oss << "             expected [" << b.GetFd() << "]" << std::endl;
		oss << "host       : result   [" << a.GetHost() << "]" << std::endl;
		oss << "             expected [" << b.GetHost() << "]" << std::endl;
		oss << "port       : result   [" << a.GetPort() << "]" << std::endl;
		oss << "             expected [" << b.GetPort() << "]" << std::endl;
		result.error_log = oss.str();
	}
	return result;
}

// test_funcを実行したらthrowされることを期待するテスト(todo: テスト共通化？)
template <typename TestFunc>
int TestThrow(
	TestFunc                  test_func,
	server::SockContext      &context,
	int                       server_fd,
	const server::ServerInfo &server_info
) {
	try {
		(context.*test_func)(server_fd, server_info);
		PrintNg();
		PrintError("throw failed");
		return EXIT_FAILURE;
	} catch (const std::exception &e) { // todo: 独自のexceptionだけcatchしてokが良いかも
		PrintOk();
		std::cerr << utils::color::GRAY << e.what() << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	}
}

// test_funcを実行したらthrowされることを期待するテスト(todo: テスト共通化？)
template <typename TestFunc>
int TestThrow(TestFunc test_func, const server::SockContext &context, int client_fd) {
	try {
		(context.*test_func)(client_fd);
		PrintNg();
		PrintError("throw failed");
		return EXIT_FAILURE;
	} catch (const std::exception &e) { // todo: 独自のexceptionだけcatchしてokが良いかも
		PrintOk();
		std::cerr << utils::color::GRAY << e.what() << utils::color::RESET << std::endl;
		return EXIT_SUCCESS;
	}
}

// -----------------------------------------------------------------------------
// test用にcontextとexpectedに同じserver_fd,server_infoを追加する
void AddServerInfoForTest(
	server::SockContext                &context,
	server::SockContext::ServerInfoMap &expected_server_info,
	int                                 server_fd,
	const server::ServerInfo           &server_info
) {
	context.AddServerInfo(server_fd, server_info);
	expected_server_info[server_fd] = server_info;
}

// test用にcontextとexpectedに同じclient,serverを追加する
void AddClientInfoForTest(
	server::SockContext                    &context,
	server::SockContext::ClientInfoMap     &expected_client_info,
	server::SockContext::HostServerInfoMap &expected_host_server_info,
	int                                     client_fd,
	const server::ClientInfo               &client_info,
	int                                     server_fd,
	const server::ServerInfo               *server_info
) {
	context.AddClientInfo(client_fd, client_info, server_fd);
	expected_client_info[client_fd]      = client_info;
	expected_host_server_info[client_fd] = server_info;
}

// test用にcontextとexpectedから同じclientを削除する
void DeleteClientInfoForTest(
	server::SockContext                    &context,
	server::SockContext::ClientInfoMap     &expected_client_info,
	server::SockContext::HostServerInfoMap &expected_host_server_info,
	int                                     client_fd
) {
	context.DeleteClientInfo(client_fd);
	expected_client_info.erase(client_fd);
	expected_host_server_info.erase(client_fd);
}

/*

以下のような接続状況を仮定しテストする

fd | listen_server |  client
--------------------------------
 4 |  ServerInfo1  |
 5 |  ServerInfo2  |
 6 |  ServerInfo1  | ClientInfo1 (accept済み接続)
 7 |  ServerInfo2  | ClientInfo2 (accept済み接続)

SockContext内部の期待されるデータ保持
- ServerInfoMap     = {{4, ServerInfo1}, {5, ServerInfo2}}
- ClientInfoMap     = {{6, ClientInfo1}, {7, ClientInfo2}}
- HostServerInfoMap = {{6, ServerInfo1*}, {7, ServerInfo2*}}

*/
int RunTestSockContext() {
	int ret_code = EXIT_SUCCESS;

	/* ----------- 準備 ----------- */
	// SockContextに渡す用のServerInfoを2個作成
	server::ServerInfo server_info1("localhost", 8080);
	server_info1.SetSockFd(4);
	server::ServerInfo server_info2("0.0.0.0", 12345);
	server_info2.SetSockFd(5);

	// SockContextに渡す用のClientInfoを2個作成
	// (todo:ClientInfoがsock_addrをコンストラクト時に渡さないといけないことによりfd,portがセットできてない)
	server::ClientInfo client_info1;
	server::ClientInfo client_info2;

	// 期待するSockContextのメンバのmap(同時に自分で作成していく)
	server::SockContext::ServerInfoMap     expected_server_info;
	server::SockContext::ClientInfoMap     expected_client_info;
	server::SockContext::HostServerInfoMap expected_host_server_info;

	/* ----------- テスト対象 : class SockContextのpublicメンバ関数 ----------- */
	server::SockContext context;

	// contextにServerInfo1を追加
	// - ServerInfoMap      = {{4, ServerInfo1}}
	// - ClientInfoMap      = {}
	// - HostServerInfopMap = {}
	const int server_fd1 = 4;
	AddServerInfoForTest(context, expected_server_info, server_fd1, server_info1);

	// contextにServerInfo2を追加
	// - ServerInfoMap      = {{4, ServerInfo1}, {5, ServerInfo2}}
	// - ClientInfoMap      = {}
	// - HostServerInfopMap = {}
	const int server_fd2 = 5;
	AddServerInfoForTest(context, expected_server_info, server_fd2, server_info2);

	// contextに既に追加済みのServerInfo2を再度追加してみる(期待: throw)
	ret_code |= TestThrow(&server::SockContext::AddServerInfo, context, server_fd2, server_info2);

	// contextにClientInfo1とそれに紐づくserver_fd1を追加
	// - ServerInfoMap     = {{4, ServerInfo1}, {5, ServerInfo2}}
	// - ClientInfoMap     = {{6, ClientInfo1}}
	// - HostServerInfoMap = {{6, ServerInfo1*}}
	const int client_fd1 = 6;
	AddClientInfoForTest(
		context,
		expected_client_info,
		expected_host_server_info,
		client_fd1,
		client_info1,
		server_fd1,
		&server_info1
	);
	// contextのメンバと自作のexpectedが同じか確認
	ret_code |= Test(RunGetClientInfo(context, expected_client_info, client_fd1));
	ret_code |= Test(RunGetConnectedServerInfo(context, expected_host_server_info, client_fd1));

	// contextにClientInfo2とServerInfoMapに登録されていないserver_fdを追加してみる (todo: 保留)
	// ret_code |= TestThrow(&server::SockContext::AddClientInfo, client_fd2, client_info2, 100);

	// contextにClientInfo2とそれに紐づくserver_fd2を追加
	// - ServerInfoMap     = {{4, ServerInfo1}, {5, ServerInfo2}}
	// - ClientInfoMap     = {{6, ClientInfo1}, {7, ClientInfo2}}
	// - HostServerInfoMap = {{6, ServerInfo1*}, {7, ServerInfo2*}}
	const int client_fd2 = 7;
	AddClientInfoForTest(
		context,
		expected_client_info,
		expected_host_server_info,
		client_fd2,
		client_info2,
		server_fd2,
		&server_info2
	);
	// contextのメンバと自作のexpectedが同じか確認
	ret_code |= Test(RunGetClientInfo(context, expected_client_info, client_fd2));
	ret_code |= Test(RunGetConnectedServerInfo(context, expected_host_server_info, client_fd2));

	// contextからClientInfo1(とそれに紐づくServerInfo1*を削除
	// (ServerInfoMapからはServerInfo1は消えない。listen待機中一覧なので)
	// - ServerInfoMap     = {{4, ServerInfo1}, {5, ServerInfo2}}
	// - ClientInfoMap     = {{7, ClientInfo2}}
	// - HostServerInfoMap = {{7, ServerInfo2*}}
	DeleteClientInfoForTest(context, expected_client_info, expected_host_server_info, client_fd1);
	// 削除後にgetterを使用し,期待通りthrowされるか確認 (todo: getterがthrowするなら必要なテスト)
	ret_code |= TestThrow(&server::SockContext::GetClientInfo, context, client_fd1);
	ret_code |= TestThrow(&server::SockContext::GetConnectedServerInfo, context, client_fd1);

	// 2度同じClientInfo1を削除してみる(期待: 何も起きない)
	context.DeleteClientInfo(client_fd1);

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestSockContext();

	return ret_code;
}
