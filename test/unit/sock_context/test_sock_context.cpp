#include "client_info.hpp"
#include "server_info.hpp"
#include "sock_context.hpp"
#include <cstdlib>
#include <iostream>
#include <string>

namespace {

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
	server::ServerInfo server_info2("abcde", 12345);
	server_info2.SetSockFd(5);

	// SockContextに渡す用のClientInfoを2個作成
	// (todo:ClientInfoがsock_addrをコンストラクト時に渡さないといけないことによりfd,portがセットできてない)
	server::ClientInfo client_info1;
	server::ClientInfo client_info2;

	// 期待するSockContextのメンバのmap(同時に自分で作成していく)
	server::SockContext::ServerInfoMap     expected_server_info;
	server::SockContext::ClientInfoMap     expected_client_info;
	server::SockContext::HostServerInfoMap expected_host_server_info;

	return ret_code;
}

} // namespace

int main() {
	int ret_code = EXIT_SUCCESS;

	ret_code |= RunTestSockContext();

	return ret_code;
}
