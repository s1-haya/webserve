// example.h
#ifndef EXAMPLE_H
#define EXAMPLE_H

class MyClass {
  public:
	MyClass(int value) : value_(value) {}

	void set_value(int value) {
		value_ = value;
	}

	int get_value() const {
		return value_;
	}

  private:
	int value_;
};

#endif
