# setup.py
from setuptools import setup, Extension
import pybind11

ext_modules = [
    Extension(
        "example",  # モジュール名
        ["bindings.cpp"],  # ソースファイル
        include_dirs=[pybind11.get_include()],  # pybind11 のインクルードディレクトリ
        language="c++",
        extra_compile_args=['-std=c++11']
    ),
]

setup(
    name="example",
    ext_modules=ext_modules,
)
