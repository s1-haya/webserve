#include "http_response.hpp"
#include "dto_server_to_http.hpp"
#include "http_message.hpp"
#include "http_parse.hpp"
#include <iostream>
#include <sstream>

namespace http {

std::string HttpResponse::Run(const HttpRequestResult &request_info) {
	HttpResponseResult response = CreateHttpResponseResult(request_info);
	return CreateHttpResponseFormat(response);
}

std::string HttpResponse::TmpRun(
	const server::DtoClientInfos &client_info,
	const server::DtoServerInfos &server_info,
	HttpRequestResult            &request_info
) {
	HttpResponseResult response =
		TmpCreateHttpResponseResult(client_info, server_info, request_info);
	return CreateHttpResponseFormat(response);
}

// todo: HttpResponseResult HttpResponse::CreateHttpResponseResult(const HttpRequestResult
// &request_info) 作成
HttpResponseResult HttpResponse::TmpCreateHttpResponseResult(
	const server::DtoClientInfos &client_info,
	const server::DtoServerInfos &server_info,
	HttpRequestResult            &request_info
) {
	try {
		// todo:
		// CheckServerInfoResult config_info = HttpServerInfoCheck::Check(server_info,
		// request_info.request);
		//     // todo: IsCgi()
		//     // - path
		//     // - cgi_extension
		//     // - method allowed
		//     if (is_cgi)
		//         // todo: cgi実行
		//         // try {
		//         //     cgi::Run()
		//         // } catch {
		//         //     cgi::Exception
		//         //     このthrowはCreateHttpResponseResult内でcatchする
		//         //     // throw Httpのエラー用に
		//         // }
		//         // response = cgi -> webserv用
		//.    else
		//       response = MethodHandler();
		//     return CreateSuccessResponseResult();
		HttpResponseResult result;
		(void)client_info;
		(void)server_info;
		(void)request_info;
		return result;
	} catch (const HttpException &e) {
		request_info.status_code = e.GetStatusCode();
		return CreateErrorHttpResponseResult(request_info);
	}
}

// mock
HttpResponseResult HttpResponse::CreateHttpResponseResult(const HttpRequestResult &request_info) {
	HttpResponseResult response;
	if (request_info.status_code != http::OK) {
		response = CreateErrorHttpResponseResult(request_info);
	} else {
		response = CreateSuccessHttpResponseResult(request_info);
	}
	return response;
}

// mock
HttpResponseResult
HttpResponse::CreateSuccessHttpResponseResult(const HttpRequestResult &request_info) {
	(void)request_info;
	HttpResponseResult response;
	response.status_line.version         = HTTP_VERSION;
	response.status_line.status_code     = "200";
	response.status_line.reason_phrase   = "OK";
	response.header_fields["Host"]       = "sawa";
	response.header_fields["Connection"] = "close";
	response.body_message = "You can't connect the dots looking forward. You can only connect the "
							"dots looking backwards";
	return response;
}

std::string HttpResponse::CreateDefaultBodyMessageFormat(
	const std::string &status_code, const std::string &reason_phrase
) {
	std::ostringstream body_message;
	body_message << "<html>" << CRLF << "<head><title>" << status_code << SP << reason_phrase
				 << "</title></head>" << CRLF << "<body>" << CRLF << "<center><h1>" << status_code
				 << SP << reason_phrase << "</h1></center>" << CRLF << "<hr><center>"
				 << SERVER_VERSION << "</center>" << CRLF << "</body>" << CRLF << "</html>" << CRLF;
	return body_message.str();
}

// mock
HttpResponseResult HttpResponse::CreateErrorHttpResponseResult(const HttpRequestResult &request_info
) {
	HttpResponseResult response;
	response.status_line.version       = HTTP_VERSION;
	response.status_line.status_code   = utils::ToString(request_info.status_code);
	response.status_line.reason_phrase = reason_phrase.at(request_info.status_code);
	// todo: StatusCodeをクラスにして、プライベートで保持する。
	// response.status_line.status_code   = request_info.status_code.GetStrStatusCode();
	// response.status_line.reason_phrase   = request_info.status_code.GetReasonPhrase();
	response.header_fields[CONTENT_TYPE] = "text/html";
	response.header_fields[SERVER]       = SERVER_VERSION;
	response.header_fields[CONNECTION]   = "close";
	response.body_message                = CreateDefaultBodyMessageFormat(
        response.status_line.status_code, response.status_line.reason_phrase
    );
	response.header_fields[CONTENT_LENGTH] = utils::ToString(response.body_message.length());
	return response;
}

std::string HttpResponse::CreateHttpResponseFormat(const HttpResponseResult &response) {
	std::ostringstream response_stream;
	response_stream << response.status_line.version << SP << response.status_line.status_code << SP
					<< response.status_line.reason_phrase << CRLF;
	typedef HeaderFields::const_iterator It;
	for (It it = response.header_fields.begin(); it != response.header_fields.end(); ++it) {
		response_stream << it->first << ":" << SP << it->second << CRLF;
	}
	response_stream << CRLF;
	response_stream << response.body_message;
	return response_stream.str();
}

} // namespace http
