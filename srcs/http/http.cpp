#include "http.hpp"
#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "http_storage.hpp"
#include "server_infos.hpp"
#include "status_code.hpp"
#include <iostream>

namespace http {

Http::Http() {}

Http::~Http() {}

HttpResult
Http::Run(const ClientInfos &client_info, const server::VirtualServerAddrList &server_info) {
	HttpResult          result;
	utils::Result<void> parsed_result =
		ParseHttpRequestFormat(client_info.fd, client_info.request_buf);
	if (!parsed_result.IsOk()) {
		return CreateBadRequestResponse(client_info);
	}
	if (IsHttpRequestFormatComplete(client_info.fd)) {
		result = CreateHttpResponse(client_info, server_info);
	}
	return result;
}

// todo
void Http::SetCgiResponse(int client_fd, const cgi::CgiResponse &cgi_response) {
	(void)client_fd;
	(void)cgi_response;
}

utils::Result<void> Http::ParseHttpRequestFormat(int client_fd, const std::string &read_buf) {
	utils::Result<void>   result;
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	save_data.current_buf += read_buf;
	try {
		HttpParse::Run(save_data);
	} catch (const HttpException &e) {
		save_data.request_result.status_code = e.GetStatusCode();
		result.Set(false);
	}
	storage_.UpdateClientSaveData(client_fd, save_data);
	return result;
}

HttpResult Http::CreateHttpResponse(
	const ClientInfos &client_info, const server::VirtualServerAddrList &server_info
) {
	HttpResult            result;
	HttpRequestParsedData data = storage_.GetClientSaveData(client_info.fd);
	result.is_connection_keep =
		HttpResponse::IsConnectionKeep(data.request_result.request.header_fields);
	result.is_response_complete = true;
	result.request_buf          = data.current_buf;
	result.response             = HttpResponse::Run(client_info, server_info, data.request_result);
	storage_.DeleteClientSaveData(client_info.fd);
	return result;
}

HttpResult Http::GetErrorResponse(const ClientInfos &client_info, ErrorState state) {
	HttpResult            result;
	HttpRequestParsedData data  = storage_.GetClientSaveData(client_info.fd);
	result.is_response_complete = true;
	result.is_connection_keep   = false;
	result.request_buf          = data.current_buf;
	switch (state) {
	case TIMEOUT:
		result.response = HttpResponse::CreateErrorResponse(StatusCode(REQUEST_TIMEOUT));
		break;
	case INTERNAL_ERROR:
		result.response = HttpResponse::CreateErrorResponse(StatusCode(INTERNAL_SERVER_ERROR));
		break;
	default:
		break;
	}
	storage_.DeleteClientSaveData(client_info.fd);
	return result;
}

HttpResult Http::CreateBadRequestResponse(const ClientInfos &client_info) {
	HttpResult            result;
	HttpRequestParsedData data  = storage_.GetClientSaveData(client_info.fd);
	result.is_response_complete = true;
	result.is_connection_keep   = false;
	result.request_buf          = data.current_buf;
	result.response             = HttpResponse::CreateErrorResponse(StatusCode(BAD_REQUEST));
	storage_.DeleteClientSaveData(client_info.fd);
	return result;
}

// todo: HTTPRequestの書式が完全かどうか(どのように取得するかは要検討)
bool Http::IsHttpRequestFormatComplete(int client_fd) {
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	return save_data.is_request_format.is_request_line &&
		   save_data.is_request_format.is_header_fields &&
		   save_data.is_request_format.is_body_message;
}

// For test
HttpRequestParsedData Http::GetClientData(int client_fd) {
	return storage_.GetClientSaveData(client_fd);
}

} // namespace http
