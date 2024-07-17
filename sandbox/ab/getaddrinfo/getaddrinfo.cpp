#include <cstdlib>
#include <cstring> // memcpy
#include <iostream>
#include <netdb.h> // getaddrinfo,freeaddrinfo
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h> // perror
#include <string>
#include <sys/socket.h> // getaddrinfo,freeaddrinfo
#include <sys/types.h>
#include <unistd.h>
#include <vector>

/*
int getaddrinfo(const char *host, const char *service, const struct addrinfo *hints, struct addrinfo
**result);

指定されたホスト名とサービス名をIPアドレスとポート番号へ変換し、ソケットアドレス構造体のリストとして返す

- host    :
ホスト名か、ドット区切りの10進数で表現したIPv4アドレスか、コロン区切りの16進数で表現したIPv6アドレスのいずれか
- service : サービス名かポート番号
- hints   : resultへ返すソケットアドレス構造体に関する指示を代入した、addrinfo構造体へのポインタ
- result  :

struct addrinfo {
	int             ai_flags;       // 入力フラグ（AI_*定数）
	int             ai_family;      // アドレスファミリ
	int             ai_socktype;    // 種類：SOCK_STREAM, SOCK_DGRAM
	int             ai_protocol;    // ソケットプロトコル
	socklen_t       ai_addrlen;     // ai_addr が指す構造体のサイズ
	char            *ai_canonname;  // 正式ホスト名
	struct sockaddr *ai_addr;       // ソケットアドレス構造体へのポインタ
	struct addrinfo *ai_next;       // リスト内の次の要素へのポインタ
}

#define __ss_aligntype uint32_t // 32 ビットアーキテクチャの場合
struct sockaddr_storage {
	sa_family_t    ss_family;
	__ss_aligntype __ss_align;               // 強制アラインメント
	char           __ss_padding[SS_PADSIZE]; // 128 バイトにパディング
};
*/

/*
void freeaddrinfo(struct addrinfo *result);

getaddrinfo()がresultへ返すアドレス構造体リストの要素は、すべてダイナミックに割り当てたメモリ。
このため、コール側ではアドレス構造体を使用後にメモリを解放する必要があり、リスト内のアドレス構造体すべてを一度に解放する
addrinfo構造体や、対応するソケットアドレス構造体を保持する必要がある場合は、freeaddrinfo()実行前にコピーする
*/

/*
int getnameinfo(const struct sockaddr *addr, socklen_t addrlen, char *host, socklen_t hostlen, char
*service, socklen_t servlen, int flags);

getaddrinfo() とは逆の機能を持ち、指定されたソケットアドレス構造体（IPv4、IPv6
のいずれか）をホスト名とサービス名へ変換します。解決できない場合は数字で表現し た文字列を返します。
*/

struct SockAddrInfo {
	std::string             ip;
	unsigned int            port;
	struct sockaddr_storage addr;
};

std::vector<SockAddrInfo> GetSockAddrInfo(const std::vector<std::string> &ports) {
	std::vector<SockAddrInfo> result;

	typedef std::vector<std::string>::const_iterator Itr;
	for (Itr it = ports.begin(); it != ports.end(); ++it) {
		struct addrinfo hints = {};
		hints.ai_socktype     = SOCK_STREAM; // TCPストリームソケット
		hints.ai_family       = AF_UNSPEC;   // Allows IPv4 or IPv6
		hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV; // 自動的に自分の IP を設定, IPv4・IPv6に対応

		const std::string &port = *it;
		struct addrinfo   *res;
		const int          status = getaddrinfo(NULL, port.c_str(), &hints, &res);
		if (status != 0) {
			std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
			continue;
		}

		for (struct addrinfo *ptr = res; ptr != NULL; ptr = ptr->ai_next) {
			int sock_fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (sock_fd == -1) {
				std::cerr << "socket error" << std::endl;
				continue;
			}

			int optval = 1;
			if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1) {
				std::cerr << "setsockopt error" << std::endl;
				close(sock_fd);
				continue;
			}

			if (bind(sock_fd, ptr->ai_addr, ptr->ai_addrlen) == -1) {
				std::cerr << "bind error" << std::endl;
				close(sock_fd);
				continue;
			}

			SockAddrInfo info;
			std::memcpy(&info.addr, ptr->ai_addr, ptr->ai_addrlen);
			socklen_t addrlen = ptr->ai_addrlen;

			// IP address, port などを取得できる
			char      ip_str[INET6_ADDRSTRLEN];
			const int getname_status = getnameinfo(
				(struct sockaddr *)&info.addr,
				addrlen,
				ip_str,
				sizeof(ip_str),
				NULL,
				0,
				NI_NUMERICHOST
			);
			if (getname_status != 0) {
				std::cerr << "getnameinfo error: " << gai_strerror(getname_status) << std::endl;
				close(sock_fd);
				continue;
			}

			info.ip = ip_str;
			if (info.addr.ss_family == AF_INET) {
				info.port = ntohs(((struct sockaddr_in *)&info.addr)->sin_port);
			} else if (info.addr.ss_family == AF_INET6) {
				info.port = ntohs(((struct sockaddr_in6 *)&info.addr)->sin6_port);
			}
			result.push_back(info);

			close(sock_fd);
		}

		// ダイナミック割り当てメモリなので開放
		freeaddrinfo(res);
	}
	return result;
}

int main() {
	std::vector<std::string> ports;
	ports.push_back("8080");
	ports.push_back("12345");

	std::vector<SockAddrInfo> sock_infos = GetSockAddrInfo(ports);

	typedef std::vector<SockAddrInfo>::const_iterator Itr;
	for (Itr it = sock_infos.begin(); it != sock_infos.end(); ++it) {
		std::cout << "IP: " << it->ip << ", Port: " << it->port << std::endl;
	}

	return 0;
}
