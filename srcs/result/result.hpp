#ifndef RESULT_HPP_
#define RESULT_HPP_

/**
 * @brief Represents the result of an operation, indicating success or failure.
 *
 * This class stores the outcome of an operation, which can either be a success or a failure.
 * If the operation is successful, the `success_value_` is stored.
 * Otherwise, only `is_success_` flag is set to `false`.
 */
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

#endif /* RESULT_HPP_ */
