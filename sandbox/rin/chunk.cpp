#include <iostream>

const std::string CRLF = "\r\n";

template <typename T>
class Result {
  public:
	Result() : is_success_(true), success_value_(T()) {}
	~Result() {}
	explicit Result(const T &success_value) : is_success_(true), success_value_(success_value) {}
	Result(bool is_success, T success_value)
		: is_success_(is_success), success_value_(success_value) {}
	Result(const Result &other) {
		*this = other;
	}
	Result &operator=(const Result &other) {
		if (this != &other) {
			is_success_    = other.is_success_;
			success_value_ = other.success_value_;
		}
		return *this;
	}
	// getter
	bool IsOk() const {
		return is_success_;
	}
	const T &GetValue() const {
		return success_value_;
	}
	// setter
	void Set(bool is_success) {
		is_success_ = is_success;
	}
	void Set(bool is_success, const T &success_value) {
		is_success_    = is_success;
		success_value_ = success_value;
	}
	void SetValue(const T &success_value) {
		success_value_ = success_value;
	}

  private:
	bool is_success_;
	T    success_value_;
};

bool IsDigit(char c) {
	return std::isdigit(static_cast<unsigned char>(c));
}

Result<unsigned int> ConvertStrToUint(const std::string &str) {
	Result<unsigned int> convert_result(false, 0);
	if (str.empty() || !IsDigit(str[0])) {
		return convert_result;
	}

	char            *end;
	static const int BASE   = 10;
	errno                   = 0;
	const unsigned long num = std::strtoul(str.c_str(), &end, BASE);
	if (errno == ERANGE || *end != '\0' || num > std::numeric_limits<unsigned int>::max()) {
		return convert_result;
	}
	convert_result.Set(true, static_cast<unsigned int>(num));
	return convert_result;
}

void ParseChunkedRequest(std::string &read_buf, std::string &body_message) {
	unsigned int length     = 0;
	unsigned int chunk_size = 0;
	// std::string::size_type start_of_chunk_pos = 0;
	do {
		std::string::size_type end_of_chunk_size_pos = read_buf.find(CRLF);
		std::string            chunk_size_str        = read_buf.substr(0, end_of_chunk_size_pos);
		read_buf.erase(0, chunk_size_str.size() + CRLF.size());
		// std::cout << read_buf << std::endl;
		chunk_size = ConvertStrToUint(chunk_size_str).GetValue();
		length += chunk_size;
		std::string::size_type end_of_chunk_data_pos = read_buf.find(CRLF);
		std::string            chunk_data            = read_buf.substr(0, end_of_chunk_data_pos);
		read_buf.erase(0, chunk_data.size() + CRLF.size());
		// std::cout << read_buf << std::endl;
		if (chunk_data.size() != chunk_size) {
			throw std::runtime_error("Error");
		}
		body_message += chunk_data;
	} while (chunk_size > 0);
	// data.is_request_format.is_body_message = true;
}

int main() {
	std::string read_buf = "4\r\n\
Wiki\r\n\
5\r\n\
pedia\r\n\
0\r\n\
\r\n";
	std::string body_message;
	ParseChunkedRequest(read_buf, body_message);
	// std::cout << read_buf << std::endl;
	std::cout << body_message << std::endl;
	// std::cout << read_buf.substr(0, 1) << std::endl;
	return 0;
}
