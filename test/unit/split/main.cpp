#include "color.hpp"
#include "split.hpp"
#include <cstdarg> // va_list
#include <cstdlib>
#include <cstring> // strlen
#include <iostream>
#include <stdexcept> // logic_error
#include <string>

namespace {
	// 比較する型
	typedef std::vector<std::string> Strs;

	// expectedのStrsを作成
	// templateのパラメータパックがc++11以降で使用不可なのでva_listを使用
	// 手動で正しいsizeをセットする必要有り
	Strs CreateExpect(std::size_t size, const char *head, ...) {
		Strs ret;

		if (size == 0) {
			return ret;
		}
		ret.push_back(std::string(head, std::strlen(head)));
		std::va_list args;
		va_start(args, head);
		for (std::size_t i = 0; i + 1 < size; i++) {
			const char *arg = va_arg(args, const char *);
			ret.push_back(std::string(arg, std::strlen(arg)));
		}
		va_end(args);
		return ret;
	}

	// SplitStr()の結果とexpectedを比較(他でも使用できそうなのでtemplateにしてみている)
	template <typename T>
	bool IsSame(const T &a, const T &b) {
		if (a.size() != b.size()) {
			return false;
		}
		typename T::const_iterator it_a = a.begin();
		typename T::const_iterator it_b = b.begin();
		while (it_a != a.end()) {
			if (*it_a != *it_b) {
				return false;
			}
			++it_a;
			++it_b;
		}
		return true;
	}

	// SplitStr()を実行してexpectedと比較
	void
	Run(const std::string &src, const std::string &substring, const Strs &expected) {
		if (IsSame(utils::SplitStr(src, substring), expected)) {
			std::cerr << COLOR_GREEN << "[OK] " << COLOR_RESET << src << std::endl;
		} else {
			std::cerr << COLOR_RED << "[NG] " << COLOR_RESET << src << std::endl;
			throw std::logic_error("SplitStr()");
		}
	}
} // namespace

int main() {
	try {
		// ---------------------------------------------------------------------
		// test for SP(" "). substring:1文字
		// ---------------------------------------------------------------------
		// substringのみのstr
		Run(" ", " ", CreateExpect(2, "", ""));
		Run("  ", " ", CreateExpect(3, "", "", ""));
		// substringがstrの中にない
		Run("", " ", CreateExpect(1, ""));
		Run("abbccc", " ", CreateExpect(1, "abbccc"));
		// substringが1つずつ
		Run("a bb ccc", " ", CreateExpect(3, "a", "bb", "ccc"));
		// substringが連続
		Run("abb   ccc", " ", CreateExpect(4, "abb", "", "", "ccc"));
		// 先頭にsubstring
		Run("  abbccc", " ", CreateExpect(3, "", "", "abbccc"));
		// 末尾にsubstring
		Run("abbccc ", " ", CreateExpect(2, "abbccc", ""));
		// 前後にsubstring
		Run(" abbccc   ", " ", CreateExpect(5, "", "abbccc", "", "", ""));

		// ---------------------------------------------------------------------
		// test for CRLF("\r\n"). substring:2文字
		// ---------------------------------------------------------------------
		// substringのみのstr
		Run("\r\n", "\r\n", CreateExpect(2, "", ""));
		Run("\r\n\r\n", "\r\n", CreateExpect(3, "", "", ""));
		// substringがstrの中にない
		Run("", "\r\n", CreateExpect(1, ""));
		Run("abbccc", "\r\n", CreateExpect(1, "abbccc"));
		// substringが1つずつ
		Run("a\r\nbb\r\nccc", "\r\n", CreateExpect(3, "a", "bb", "ccc"));
		// substringが連続
		Run("abb\r\n\r\nccc", "\r\n", CreateExpect(3, "abb", "", "ccc"));
		// 先頭にsubstring
		Run("\r\nabbccc", "\r\n", CreateExpect(2, "", "abbccc"));
		// 末尾にsubstring
		Run("abbccc\r\n", "\r\n", CreateExpect(2, "abbccc", ""));
		// 前後にsubstring
		Run("\r\nabbccc\r\n\r\n", "\r\n", CreateExpect(4, "", "abbccc", "", ""));
		// substringの中のcharがあっても何も起こらない
		Run("a\rbb\r\nccc", "\r\n", CreateExpect(2, "a\rbb", "ccc"));
		Run("a\nbb\r\nccc", "\r\n", CreateExpect(2, "a\nbb", "ccc"));

		// ---------------------------------------------------------------------
		// test for another case
		// ---------------------------------------------------------------------
		// substringが空文字列
		Run("", "", CreateExpect(1, ""));
		Run("abc", "", CreateExpect(1, "abc"));

	} catch (const std::exception &e) {
		std::cerr << COLOR_RED << "Error: " << e.what() << COLOR_RESET << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
