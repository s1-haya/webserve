#include <iostream>
#include <map>
#include <string>
#include <vector>

// パートを表す構造体
struct Part {
	std::map<std::string, std::string> headers;
	std::string                        body;
};

// Boundaryを抽出する関数
std::string ExtractBoundary(const std::string &content_type) {
	std::string boundary_prefix = "boundary=";
	size_t      pos             = content_type.find(boundary_prefix);
	if (pos != std::string::npos) {
		return "--" + content_type.substr(pos + boundary_prefix.length());
	}
	return "";
}

// パートを分割する関数
std::vector<std::string> SplitParts(const std::string &body, const std::string &boundary) {
	std::vector<std::string> parts;
	size_t start = body.find(boundary) + boundary.length() + 2; // 修正: +2は\r\nをスキップするため
	size_t end = body.find(boundary, start);
	while (end != std::string::npos) {
		// std::cout << "aa    " << body.substr(start, end - start - 2) << std::endl; // 修正:
		// -2は\r\nを除外するため
		parts.push_back(body.substr(start, end - start - 2)); // 修正: -2は\r\nを除外するため
		start = end + boundary.length() + 2; // 修正: +2は\r\nをスキップするため
		end   = body.find(boundary, start);
	}
	// for (std::vector<std::string>::const_iterator part = parts.begin(); part != parts.end();
	// ++part) {
	//     std::cout << "part: " << *part << std::endl;
	// } // ok
	return parts;
}

// ヘッダーとボディを分割する関数
Part ParsePart(const std::string &part) {
	Part   result;
	size_t header_end = part.find("\r\n\r\n");
	if (header_end != std::string::npos) {
		std::string headers = part.substr(0, header_end);
		// std::cout << "headers: " << headers << std::endl; // ok
		result.body = part.substr(header_end + 4);
		// std::cout << "body: " << result.body << std::endl; // ok

		size_t pos = 0;
		size_t end = headers.find("\r\n", pos);
		while (end != std::string::npos) {
			std::string header = headers.substr(pos, end - pos);
			size_t      colon  = header.find(": ");
			if (colon != std::string::npos) {
				std::string name     = header.substr(0, colon);
				std::string value    = header.substr(colon + 2);
				result.headers[name] = value;
			}
			pos = end + 2;
			end = headers.find("\r\n", pos);
		}
		// 最後のヘッダー行を処理
		std::string last_header = headers.substr(pos);
		size_t      colon       = last_header.find(": ");
		if (colon != std::string::npos) {
			std::string name     = last_header.substr(0, colon);
			std::string value    = last_header.substr(colon + 2);
			result.headers[name] = value;
		}
	}
	return result;
}

// multipart/form-dataをデコードする関数
std::vector<Part>
DecodeMultipartFormData(const std::string &content_type, const std::string &body) {
	std::vector<Part> parts;
	std::string       boundary = ExtractBoundary(content_type);
	if (!boundary.empty()) {
		std::vector<std::string> raw_parts = SplitParts(body, boundary);
		for (std::vector<std::string>::const_iterator raw_part = raw_parts.begin();
			 raw_part != raw_parts.end();
			 ++raw_part) {
			Part part = ParsePart(*raw_part);
			if (!part.headers.empty()) {
				parts.push_back(part);
			}
		}
	}
	return parts;
}

std::string content_type = "multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW";
std::string body         = "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
						   "Content-Disposition: form-data; name=\"field1\"\r\n\r\n"
						   "value1\r\n"
						   "------WebKitFormBoundary7MA4YWxkTrZu0gW\r\n"
						   "Content-Disposition: form-data; name=\"field2\"; filename=\"example.txt\"\r\n"
						   "Content-Type: text/plain\r\n\r\n"
						   "example content\r\n"
						   "------WebKitFormBoundary7MA4YWxkTrZu0gW--";

std::vector<Part> parts = DecodeMultipartFormData(content_type, body);

int main() {
	for (std::vector<Part>::const_iterator part_it = parts.begin(); part_it != parts.end();
		 ++part_it) {
		for (std::map<std::string, std::string>::const_iterator header_it =
				 part_it->headers.begin();
			 header_it != part_it->headers.end();
			 ++header_it) {
			std::cout << header_it->first << ": " << header_it->second << std::endl;
		}
		std::cout << "Body: " << part_it->body << std::endl;
	}
	return 0;
}
