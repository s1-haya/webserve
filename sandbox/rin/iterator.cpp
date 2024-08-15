#include <iostream>
#include <stdexcept>
#include <vector>

enum IncrementMode {
	THROW_ON_END,
	ALLOW_END
};

template <typename T>
class CustomIterator {
  public:
	CustomIterator(T *ptr, T *end) : current(ptr), end(end), mode(THROW_ON_END) {}

	// インクリメント演算子のオーバーロード
	CustomIterator &operator++() {
		if (current == end) {
			if (mode == THROW_ON_END) {
				throw std::runtime_error("unexpected end of server context");
			}
		} else {
			++current;
		}
		return *this;
	}

	// 状態を変更するメソッド
	void setMode(IncrementMode newMode) {
		mode = newMode;
	}

	// デリファレンス演算子
	T &operator*() {
		return *current;
	}

	// 比較演算子
	bool operator==(const CustomIterator &other) const {
		return current == other.current;
	}
	bool operator!=(const CustomIterator &other) const {
		return !(*this == other);
	}

  private:
	T            *current;
	T            *end;
	IncrementMode mode;
};

template <typename T>
class CustomContainer {
  public:
	CustomContainer(const std::vector<T> &vec) : data(vec) {}

	CustomIterator<T> begin() {
		return CustomIterator<T>(data.data(), data.data() + data.size());
	}

	CustomIterator<T> end() {
		return CustomIterator<T>(data.data() + data.size(), data.data() + data.size());
	}

  private:
	std::vector<T> data;
};

int main() {
	std::vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	CustomContainer<int> container(vec);

	// インクリメント時の動作を選択する例
	CustomIterator<int> it = container.begin();
	it.setMode(THROW_ON_END); // デフォルトで例外をスローする設定

	try {
		++it;
		++it;
		++it;
		++it;
		++it; // これで `end` まで進む
		++it; // ここで例外が発生
	} catch (const std::runtime_error &e) {
		std::cout << e.what() << std::endl;
	}

	// モードを変更して例外が発生しないようにする
	it.setMode(ALLOW_END);
	++it; // ここで例外は発生しない
	std::cout << "No exception when advancing to end with ALLOW_END mode." << std::endl;

	return 0;
}
