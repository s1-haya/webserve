#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_parse.hpp"
#include "http_response.hpp"
#include <cstdlib>
#include <fstream>

namespace {

std::string LoadFileContent(const char *file_path) {
	std::ifstream file(file_path, std::ios::binary);
	if (!file) {
		std::cerr << "Error opening file: " << file_path << std::endl;
		return "";
	}
	std::ostringstream file_content;
	file_content << file.rdbuf();
	return file_content.str();
}

int GetTestCaseNum() {
	static unsigned int test_case_num = 0;
	++test_case_num;
	return test_case_num;
}

template <typename T>
int HandleResult(const T &result, const T &expected) {
	if (result == expected) {
		std::cout << utils::color::GREEN << GetTestCaseNum() << ".[OK]" << utils::color::RESET
				  << std::endl;
		return EXIT_SUCCESS;
	} else {
		std::cerr << utils::color::RED << GetTestCaseNum() << ".[NG] " << utils::color::RESET
				  << std::endl;
		std::cerr << "Error: Http Response\n";
		std::cerr << "- Expected:\n[" << expected << "]\n";
		std::cerr << "- Result  :\n[" << result << "]\n";
		return EXIT_FAILURE;
	}
}

server::Location BuildLocation(
	const std::string                          &request_uri,
	const std::string                          &alias,
	const std::string                          &index,
	bool                                        autoindex,
	const std::list<std::string>               &allowed_methods,
	const std::pair<unsigned int, std::string> &redirect,
	const std::string                          &cgi_extension    = "",
	const std::string                          &upload_directory = ""
) {
	server::Location loc;
	loc.request_uri      = request_uri;
	loc.alias            = alias;
	loc.index            = index;
	loc.autoindex        = autoindex;
	loc.allowed_methods  = allowed_methods;
	loc.redirect         = redirect;
	loc.cgi_extension    = cgi_extension;
	loc.upload_directory = upload_directory;
	return loc;
}

server::VirtualServer *BuildVirtualServer1() {
	// LocationList
	server::VirtualServer::LocationList locationlist;
	// リソースの取得位置によって(srcs/http/response/http_method.cpp)によって出力結果が決まる
	std::string                         alias = "/html/";
	server::Location::AllowedMethodList allowed_methods;
	allowed_methods.push_back("GET");
	allowed_methods.push_back("POST");
	server::Location::Redirect redirect;
	server::Location::Redirect redirect_on(301, "/");
	server::Location           location1 =
		BuildLocation("/", alias, "index.html", false, allowed_methods, redirect);
	server::Location location2 = // redirect_on
		BuildLocation("/www/", alias, "index.html", true, allowed_methods, redirect_on);
	locationlist.push_back(location1);
	locationlist.push_back(location2);

	server::VirtualServer                 virtual_server;
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host1");
	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("localhost", 8080));
	server::VirtualServer::ErrorPage error_page(404, "/404.html");

	return new server::VirtualServer(server_names, locationlist, host_ports, 1024, error_page);
}

server::VirtualServer *BuildVirtualServer2() {
	// LocationList
	server::VirtualServer::LocationList locationlist;
	// リソースの取得位置によって(srcs/http/response/http_method.cpp)によって出力結果が決まる
	std::string                         alias = "../../../../root/html/index.html";
	server::Location::AllowedMethodList allowed_methods;
	allowed_methods.push_back("POST");
	server::Location::Redirect redirect;
	server::Location::Redirect redirect_on(301, "/");
	server::Location           location1 =
		BuildLocation("/", alias, "index.html", false, allowed_methods, redirect);
	server::Location location2 = // redirect_on
		BuildLocation("/www/", alias, "index.html", true, allowed_methods, redirect_on);
	locationlist.push_back(location1);
	locationlist.push_back(location2);

	server::VirtualServer                 virtual_server;
	server::VirtualServer::ServerNameList server_names;
	server_names.push_back("host2");
	server::VirtualServer::HostPortList host_ports;
	host_ports.push_back(std::make_pair("localhost", 8080));
	server::VirtualServer::ErrorPage error_page(404, "/404.html");

	return new server::VirtualServer(server_names, locationlist, host_ports, 1024, error_page);
}

server::VirtualServerAddrList BuildVirtualServerAddrList() {
	server::VirtualServerAddrList virtual_servers;
	server::VirtualServer        *vs1 = BuildVirtualServer1();
	server::VirtualServer        *vs2 = BuildVirtualServer2();
	virtual_servers.push_back(vs1);
	virtual_servers.push_back(vs2);
	return virtual_servers;
}

void DeleteAddrList(const server::VirtualServerAddrList &virtual_servers) {
	typedef server::VirtualServerAddrList::const_iterator ItVirtualServer;
	for (ItVirtualServer it = virtual_servers.begin(); it != virtual_servers.end(); ++it) {
		delete *it;
	}
}

std::string SetDefaultHeaderFields(
	const std::string &connection, const std::string &length, const std::string &type
) {
	std::string header_fields;
	header_fields += http::CONNECTION + ": " + connection + http::CRLF;
	header_fields += http::CONTENT_LENGTH + ": " + length + http::CRLF;
	header_fields += http::CONTENT_TYPE + ": " + type + http::CRLF;
	header_fields += http::SERVER + ": " + http::SERVER_VERSION + http::CRLF;
	return header_fields;
}

} // namespace

int main(void) {
	int                                 ret_code = 0;
	http::ClientInfos                   client_info;
	const server::VirtualServerAddrList server_info = BuildVirtualServerAddrList();
	http::HttpRequestResult             request_info;

	// 前提
	// header_fields[HOST]がないとAborted what():  map::at
	// VirtualServerLAddrListがないとセグフォ

	// host1に/html/index.htmlを取得するリクエスト
	request_info.request.request_line.method         = http::GET;
	request_info.request.request_line.request_target = "/";
	request_info.request.request_line.version        = http::HTTP_VERSION;
	request_info.request.header_fields[http::HOST]   = "sawa";
	std::string response1 = http::HttpResponse::Run(client_info, server_info, request_info);

	std::string expected1_status_line =
		LoadFileContent("../../expected_response/default_status_line/200_ok.txt");
	std::string expected1_body_message  = LoadFileContent("../../../../root/html/index.html");
	std::string expected1_header_fields = SetDefaultHeaderFields(
		http::KEEP_ALIVE, utils::ToString(expected1_body_message.length()), "test/html"
	);
	const std::string &expected1_response =
		expected1_status_line + expected1_header_fields + http::CRLF + expected1_body_message;

	ret_code |= HandleResult(response1, expected1_response);

	// GETメソッドの許可がないhost2に/html/index.htmlを取得するリクエスト
	request_info.request.header_fields[http::HOST] = "host2";
	std::string response2 = http::HttpResponse::Run(client_info, server_info, request_info);

	std::string expected2_status_line =
		LoadFileContent("../../expected_response/default_status_line/501_not_implemented.txt");
	std::string expected2_body_message =
		LoadFileContent("../../expected_response/default_body_message/501_not_implemented.txt");
	std::string expected2_header_fields = SetDefaultHeaderFields(
		http::KEEP_ALIVE, utils::ToString(expected2_body_message.length()), "test/html"
	);
	const std::string &expected2_response =
		expected2_status_line + expected2_header_fields + http::CRLF + expected2_body_message;
	ret_code |= HandleResult(response2, expected2_response);

	DeleteAddrList(server_info);
	return ret_code;
}
