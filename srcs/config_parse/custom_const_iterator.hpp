#ifndef CUSTOM_CONST_ITERATOR_HPP_
#define CUSTOM_CONST_ITERATOR_HPP_

#include <list>
#include <stdexcept>

namespace config {

/**
 * @brief Custom Iterator for ConfigParser which can Throw on unexpected end
 *
 * @tparam std::list<T>::const_iterator
 * @note The Iterator is only compatible with a `const std::list<T>`
 */
template <typename T>
class CustomConstIterator {
  public:
	CustomConstIterator(
		typename std::list<T>::const_iterator it,
		typename std::list<T>::const_iterator end,
		bool                                  throw_on_end = true
	)
		: it_(it), end_(end), throw_on_end_(throw_on_end) {};
	~CustomConstIterator() {};

	CustomConstIterator &operator++() {
		if (it_ == end_ && throw_on_end_) {
			throw std::out_of_range("unexpected end");
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
		return !(*this == other);
	}

	bool operator==(const typename std::list<T>::const_iterator &other) const {
		return it_ == other;
	}
	bool operator!=(const typename std::list<T>::const_iterator &other) const {
		return it_ != other;
	}

  private:
	typename std::list<T>::const_iterator it_;
	typename std::list<T>::const_iterator end_;
	bool                                  throw_on_end_;
};

} // namespace config

#endif
