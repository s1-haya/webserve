#include "http.hpp"
#include "client_infos.hpp"
#include "http_message.hpp"
#include "http_result.hpp"
#include "http_storage.hpp"
#include "server_infos.hpp"
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
		return CreateBadRequestResponse(client_info.fd);
	}
	if (IsHttpRequestFormatComplete(client_info.fd)) {
		// todo: request_buf, is_connection_keep
		result.is_connection_keep   = IsConnectionKeep(client_info.fd);
		result.is_response_complete = true;
		result.response             = CreateHttpResponse(client_info, server_info);
	}
	return result;
}

HttpResult Http::GetErrorResponse(const ClientInfos &client_info, ErrState state) {
	HttpResult result;
	(void)client_info;
	(void)state;
	return result;
}

utils::Result<void> Http::ParseHttpRequestFormat(int client_fd, const std::string &read_buf) {
	utils::Result<void>   result;
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	save_data.current_buf += read_buf;
	try {
		HttpParse::TmpRunHttpResultVersion(save_data);
	} catch (const HttpException &e) {
		save_data.request_result.status_code = e.GetStatusCode();
		result.Set(false);
	}
	storage_.UpdateClientSaveData(client_fd, save_data);
	return result;
}

// HttpResult CreateTimeoutRequest(client_fd) {
// HttpResult result;
// HttpRequestParsedData data = storage_.GetClientSaveData(client_fd);
// result.is_response_complete = true;
// result.response = HttpResponse::CreateTimeoutRequest(data.request_result);
// result.request_buf = data.request_buf;
// todo: HttpResponse::IsConnectionKeep
// result.is_connection_keep = ;
// storage_.DeleteClientSaveData(client_fd);
// return result;
// }

// HttpResult CreateInternalServerError(client_fd)
// HttpResult result;
// HttpRequestParsedData data = storage_.GetClientSaveData(client_fd);
// result.is_response_complete = true;
// result.response = HttpResponse::CreateInternalServerError(data.request_result);;
// result.request_buf = data.request_buf;
// todo: HttpResponse::IsConnectionKeep
// result.is_connection_keep = ;
// storage_.DeleteClientSaveData(client_fd);
// return result;
// }

HttpResult Http::CreateBadRequestResponse(int client_fd) {
	HttpResult            result;
	HttpRequestParsedData data  = storage_.GetClientSaveData(client_fd);
	result.is_response_complete = false;
	// todo: BadRequestの場合はkeep-aliveにするかcloseにするか(現在はclose)
	result.is_connection_keep = false;
	result.request_buf = data.current_buf;
	result.response             = HttpResponse::CreateBadRequestResponse(data.request_result);
	storage_.DeleteClientSaveData(client_fd);
	return result;
}

std::string Http::CreateHttpResponse(
	const ClientInfos &client_info, const server::VirtualServerAddrList &server_info
) {
	HttpRequestParsedData data = storage_.GetClientSaveData(client_info.fd);
	storage_.DeleteClientSaveData(client_info.fd);
	return HttpResponse::Run(client_info, server_info, data.request_result);
}

// todo: HTTPRequestの書式が完全かどうか(どのように取得するかは要検討)
bool Http::IsHttpRequestFormatComplete(int client_fd) {
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	return save_data.is_request_format.is_request_line &&
		   save_data.is_request_format.is_header_fields &&
		   save_data.is_request_format.is_body_message;
}

bool Http::IsConnectionKeep(int client_fd) {
	HttpRequestParsedData save_data = storage_.GetClientSaveData(client_fd);
	return HttpResponse::IsConnectionKeep(save_data.request_result.request.header_fields);
}

// For test
HttpRequestParsedData Http::GetClientData(int client_fd) {
	return storage_.GetClientSaveData(client_fd);
}

} // namespace http
