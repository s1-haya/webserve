#ifndef CUSTOM_CONST_ITERATOR_HPP_
#define CUSTOM_CONST_ITERATOR_HPP_

#include <list>
#include <stdexcept>

namespace config {

enum IncrementMode {
	THROW_ON_END,
	ALLOW_END
};

template <typename T>
class CustomConstIterator {
  public:
	CustomConstIterator(
		typename std::list<T>::const_iterator it, typename std::list<T>::const_iterator end
	)
		: it_(it), end_(end), mode_(THROW_ON_END) {};
	~CustomConstIterator() {};

	CustomConstIterator &operator++() {
		if (it_ == end_ && mode_ == THROW_ON_END) {
			throw std::runtime_error("unexpected end");
		} else {
			++it_;
		}
		return *this;
	}

	CustomConstIterator operator++(int) {
		CustomConstIterator temp = *this;
		++(*this);
		return temp;
	}

	const T &operator*() const {
		return *it_;
	}

	bool operator==(const CustomConstIterator &other) const {
		return it_ == other.it_;
	}
	bool operator!=(const CustomConstIterator &other) const {
		return it_ != other.it_;
	}

	bool operator==(const typename std::list<T>::const_iterator &other) const {
		return it_ == other;
	}
	bool operator!=(const typename std::list<T>::const_iterator &other) const {
		return it_ != other;
	}

	void SetMode(IncrementMode mode) {
		mode_ = mode;
	}

  private:
	typename std::list<T>::const_iterator it_;
	typename std::list<T>::const_iterator end_;
	IncrementMode                         mode_;
};

} // namespace config

#endif
