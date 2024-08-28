// bindings.cpp
#include "example.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(example, m) {
	py::class_<MyClass>(m, "MyClass")
		.def(py::init<int>())                   // コンストラクタ
		.def("set_value", &MyClass::set_value)  // メソッドのバインド
		.def("get_value", &MyClass::get_value); // メソッドのバインド
}
