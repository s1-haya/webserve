#include "http_parse.hpp"
#include "http_message.hpp"
#include "utils.hpp"
#include <algorithm> // std::find
#include <vector>

namespace http {

namespace {

bool IsUsAscii(int c) {
	return static_cast<unsigned char>(c) > 127;
}

bool IsStringUsAscii(const std::string &str) {
	typedef std::string::const_iterator It;
	for (It it = str.begin(); it != str.end(); ++it) {
		if (IsUsAscii(*it)) {
			return false;
		}
	}
	return true;
}

bool IsStringUpper(const std::string &str) {
	typedef std::string::const_iterator It;
	for (It it = str.begin(); it != str.end(); ++it) {
		if (!std::isupper(static_cast<unsigned char>(*it))) {
			return false;
		}
	}
	return true;
}

std::string StrTrimOptionalWhitespace(const std::string &str) {
	std::string::size_type start = str.find_first_not_of(OPTIONAL_WHITESPACE);
	if (start == std::string::npos) {
		return "";
	}
	std::string::size_type end = str.find_last_not_of(OPTIONAL_WHITESPACE);
	return str.substr(start, end - start + 1);
}

bool IsBodyMessageReadingRequired(const HeaderFields &header_fields) {
	if (header_fields.find(CONTENT_LENGTH) == header_fields.end() &&
		header_fields.find(TRANSFER_ENCODING) == header_fields.end()) {
		return false;
	}
	return true;
}

// 大文字と小文字は区別なし
utils::Result<int> HexToDec(const std::string &hex_str) {
	std::istringstream iss(hex_str);
	int                decimal_value;
	if (!(iss >> std::hex >> decimal_value) || decimal_value < 0) {
		return utils::Result<int>(false, -1);
	}
	return utils::Result<int>(decimal_value);
}

bool HasSpace(const std::string &str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (std::isspace(*it)) {
			return true;
		}
	}
	return false;
}

// 各文字が印字可能文字(VCHR)かどうかをチェック
bool IsVString(const std::string &str) {
	for (std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
		if (!std::isprint(*it)) {
			return false;
		}
	}
	return true;
}

typedef utils::Result<std::pair<std::string::size_type, std::string> > ChunkSizeResult;
typedef utils::Result<std::string>                                     ChunkDataResult;

ChunkSizeResult GetChunkSizeStr(const std::string &current_buf) {
	ChunkSizeResult result;

	const std::string::size_type end_of_chunk_size_pos = current_buf.find(CRLF);
	if (end_of_chunk_size_pos == std::string::npos) {
		if (current_buf.size() > 8) { // INT_MAX: 7FFFFFFF(8digits)
			throw HttpException("Error: incorrect chunk size", StatusCode(BAD_REQUEST));
		}
		result.Set(false);
		return result;
	}
	const std::string chunk_size_str = current_buf.substr(0, end_of_chunk_size_pos);
	if (!HexToDec(chunk_size_str).IsOk()) {
		throw HttpException(
			"Error: chunk size is not a hexadecimal number", StatusCode(BAD_REQUEST)
		);
	}
	result.SetValue(std::make_pair(end_of_chunk_size_pos, chunk_size_str));
	return result;
}

ChunkDataResult GetChunkData(
	const std::string     &current_buf,
	std::string::size_type end_of_chunk_size_pos,
	std::size_t            chunk_size
) {
	ChunkDataResult result;

	// "chunk_size\r\n"の次の文字からfind
	const std::string::size_type end_of_chunk_data_pos =
		current_buf.find(CRLF, end_of_chunk_size_pos + CRLF.size());
	if (end_of_chunk_data_pos == std::string::npos) {
		// CRLFがないかつ"3\r\nXXX\rX"のようにchunk_date以降が(chunk_size+1)より多かったら早期に400
		if (current_buf.size() > end_of_chunk_size_pos + CRLF.size() + chunk_size + 1) {
			throw HttpException(
				"Error: Missing or incorrect chunked transfer encoding terminator",
				StatusCode(BAD_REQUEST)
			);
		}
		result.Set(false);
		return result;
	}
	const std::string chunk_data = current_buf.substr(
		end_of_chunk_size_pos + CRLF.size(),
		end_of_chunk_data_pos - end_of_chunk_size_pos - CRLF.size()
	);
	result.SetValue(chunk_data);
	return result;
}

void ThrowMissingHostHeaderField(const HeaderFields &header_fields) {
	if (header_fields.count(HOST) == 0) {
		throw HttpException("Error: missing Host header field.", StatusCode(BAD_REQUEST));
	}
}

void ValidateInvalidHeaderFields(const HeaderFields &header_fields, const std::string &method) {
	if (method == POST && !header_fields.count(CONTENT_TYPE)) {
		throw HttpException("Error: missing Content-Type header field.", StatusCode(BAD_REQUEST));
	}
	if (method == POST && !IsBodyMessageReadingRequired(header_fields)) {
		throw HttpException("Error: missing Content-Length header field.", StatusCode(BAD_REQUEST));
	}
}

} // namespace

void HttpParse::ParseRequestLine(HttpRequestParsedData &data) {
	if (data.is_request_format.is_request_line) {
		return;
	}
	size_t pos = data.current_buf.find(CRLF);
	if (pos == std::string::npos) {
		return;
	}
	std::string request_line = data.current_buf.substr(0, pos);
	data.current_buf.erase(0, pos + CRLF.size());
	data.request_result.request.request_line = SetRequestLine(utils::SplitStr(request_line, SP));
	data.is_request_format.is_request_line   = true;
}

void HttpParse::ParseHeaderFields(HttpRequestParsedData &data) {
	if (!data.is_request_format.is_request_line) {
		return;
	}
	if (data.is_request_format.is_header_fields) {
		return;
	}
	size_t pos = data.current_buf.find(HEADER_FIELDS_END);
	if (pos == std::string::npos) {
		return;
	}
	std::string header_fields = data.current_buf.substr(0, pos);
	data.current_buf.erase(0, pos + HEADER_FIELDS_END.size());
	data.request_result.request.header_fields =
		SetHeaderFields(utils::SplitStr(header_fields, CRLF));
	ValidateInvalidHeaderFields(
		data.request_result.request.header_fields, data.request_result.request.request_line.method
	);
	data.is_request_format.is_header_fields = true;
	if (!IsBodyMessageReadingRequired(data.request_result.request.header_fields)) {
		data.is_request_format.is_body_message = true;
	}
}

void HttpParse::ParseBodyMessage(HttpRequestParsedData &data) {
	if (!data.is_request_format.is_request_line) {
		return;
	}
	if (!data.is_request_format.is_header_fields) {
		return;
	}
	if (data.is_request_format.is_body_message) {
		return;
	}
	if (data.request_result.request.header_fields.find(TRANSFER_ENCODING) !=
			data.request_result.request.header_fields.end() &&
		data.request_result.request.header_fields.at(TRANSFER_ENCODING) == CHUNKED) {
		ParseChunkedRequest(data);
		return;
	}
	// todo: HttpRequestParsedDataクラスでcontent_lengthを保持？
	// why: ParseBodyMessageが呼ばれるたびにcontent_lengthを変換するのを避けるため
	const size_t content_length =
		utils::ConvertStrToSize(data.request_result.request.header_fields[CONTENT_LENGTH])
			.GetValue();
	size_t readable_content_length =
		content_length - data.request_result.request.body_message.size();
	if (data.current_buf.size() >= readable_content_length) {
		data.request_result.request.body_message +=
			data.current_buf.substr(0, readable_content_length);
		data.current_buf.erase(0, readable_content_length);
		data.is_request_format.is_body_message = true;
	} else {
		data.request_result.request.body_message += data.current_buf;
		data.current_buf.clear();
	}
}

void HttpParse::ParseChunkedRequest(HttpRequestParsedData &data) {
	if (data.request_result.request.header_fields.find(CONTENT_LENGTH) !=
		data.request_result.request.header_fields.end()) {
		throw HttpException(
			"Error: Content-Length and Transfer-Encoding are both specified",
			StatusCode(BAD_REQUEST)
		);
	}

	ChunkSizeResult result = GetChunkSizeStr(data.current_buf);
	if (!result.IsOk()) {
		return;
	}
	std::string::size_type end_of_chunk_size_pos = result.GetValue().first;
	std::string            chunk_size_str        = result.GetValue().second;
	std::size_t            chunk_size            = HexToDec(chunk_size_str).GetValue();

	while (true) {
		const ChunkDataResult chunk_data_result =
			GetChunkData(data.current_buf, end_of_chunk_size_pos, chunk_size);
		if (!chunk_data_result.IsOk()) {
			return;
		}
		const std::string chunk_data = chunk_data_result.GetValue();
		if (chunk_data.size() != chunk_size) {
			throw HttpException(
				"Error: chunk size and chunk data size are different", StatusCode(BAD_REQUEST)
			);
		}
		// sizeとdataが揃ったのでbody_messageに追加 & current_bufからまとめてerase
		data.request_result.request.body_message += chunk_data;
		const std::size_t chunk_size_and_data_length =
			chunk_size_str.size() + CRLF.size() + chunk_data.size() + CRLF.size();
		data.current_buf.erase(0, chunk_size_and_data_length);
		if (chunk_size == 0) {
			break;
		}

		ChunkSizeResult result = GetChunkSizeStr(data.current_buf);
		if (!result.IsOk()) {
			return;
		}
		end_of_chunk_size_pos = result.GetValue().first;
		chunk_size_str        = result.GetValue().second;
		chunk_size            = HexToDec(chunk_size_str).GetValue();
	}

	data.is_request_format.is_body_message = true;
}

void HttpParse::Run(HttpRequestParsedData &data) {
	ParseRequestLine(data);
	ParseHeaderFields(data);
	ParseBodyMessage(data);
}

RequestLine HttpParse::SetRequestLine(const std::vector<std::string> &request_line_info) {
	CheckValidRequestLine(request_line_info);
	RequestLine request_line;
	request_line.method         = request_line_info[0];
	request_line.request_target = request_line_info[1];
	request_line.version        = request_line_info[2];
	return request_line;
}

HeaderFields HttpParse::SetHeaderFields(const std::vector<std::string> &header_fields_info) {
	HeaderFields                                     header_fields;
	typedef std::vector<std::string>::const_iterator It;
	for (It it = header_fields_info.begin(); it != header_fields_info.end(); ++it) {
		std::size_t colon_pos          = (*it).find_first_of(':');
		std::string header_field_name  = utils::ToLowerString((*it).substr(0, colon_pos));
		std::string header_field_value = utils::ToLowerString((*it).substr(colon_pos + 1));
		header_field_value             = StrTrimOptionalWhitespace(header_field_value);
		CheckValidHeaderFieldNameAndValue(header_field_name, header_field_value);
		// todo:
		// マルチパートを対応する場合はutils::SplitStrを使用して、セミコロン区切りのstd::vector<std::string>になる。
		// ex) Content-Type: multipart/form-data; boundary=----WebKitFormBoundary64XhQJfFNRKx7oK7
		typedef std::pair<HeaderFields::const_iterator, bool> Result;
		Result result = header_fields.insert(std::make_pair(header_field_name, header_field_value));
		if (result.second == false) {
			throw HttpException(
				"Error: The value already exists in header fields", StatusCode(BAD_REQUEST)
			);
		}
	}
	ThrowMissingHostHeaderField(header_fields);
	return header_fields;
}

void HttpParse::CheckValidRequestLine(const std::vector<std::string> &request_line_info) {
	if (request_line_info.size() != 3) {
		throw HttpException(
			"Error: invalid number of status line elements", StatusCode(BAD_REQUEST)
		);
	}
	CheckValidMethod(request_line_info[0]);
	CheckValidRequestTarget(request_line_info[1]);
	CheckValidVersion(request_line_info[2]);
}

void HttpParse::CheckValidMethod(const std::string &method) {
	if (!method.size()) {
		throw HttpException("Error: the method don't exist.", StatusCode(BAD_REQUEST));
	}
	if (IsStringUsAscii(method) == false || IsStringUpper(method) == false) {
		throw HttpException(
			"Error: This method contains lowercase or non-USASCII characters.",
			StatusCode(BAD_REQUEST)
		);
	}
}

void HttpParse::CheckValidRequestTarget(const std::string &request_target) {
	if (!request_target.size()) {
		throw HttpException("Error: the request target don't exist.", StatusCode(BAD_REQUEST));
	}
	if (request_target.empty() || request_target[0] != '/') {
		throw HttpException(
			"Error: the request target is missing the '/' character at the beginning",
			StatusCode(BAD_REQUEST)
		);
	}
}

void HttpParse::CheckValidVersion(const std::string &version) {
	if (!version.size()) {
		throw HttpException("Error: the http version don't exist.", StatusCode(BAD_REQUEST));
	}
	if (version != HTTP_VERSION) {
		throw HttpException(
			"Error: The version is not supported by webserv", StatusCode(BAD_REQUEST)
		);
	}
}

void HttpParse::CheckValidHeaderFieldNameAndValue(
	const std::string &header_field_name, const std::string &header_field_value
) {
	if (!header_field_name.size()) {
		throw HttpException(
			"Error: the name of Header field don't exist.", StatusCode(BAD_REQUEST)
		);
	}
	if (!IsVString(header_field_name) || !IsVString(header_field_value)) {
		throw HttpException(
			"Error: the name or value of Header field contains non-VCHR characters.",
			StatusCode(BAD_REQUEST)
		);
	}
	if (HasSpace(header_field_name)) {
		throw HttpException(
			"Error: the name of Header field has a space.", StatusCode(BAD_REQUEST)
		);
	}
	if (header_field_name == HOST && header_field_value.empty()) {
		throw HttpException(
			"Error: the value of Host header field is empty.", StatusCode(BAD_REQUEST)
		);
	} else if (header_field_name == CONTENT_LENGTH && !utils::ConvertStrToSize(header_field_value).IsOk()) {
		throw HttpException(
			"Error: the value of Content-Length header field is not a number.",
			StatusCode(BAD_REQUEST)
		);
	}
}

// status_line && header
// message-body

} // namespace http
