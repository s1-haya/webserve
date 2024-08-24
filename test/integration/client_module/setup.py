from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext


class get_pybind_include(object):
    """Helper class to determine the pybind11 include path
    The purpose of this class is to postpone importing pybind11
    until it is actually installed, so that the `get_include()`
    method can be invoked."""

    def __str__(self):
        import pybind11

        return pybind11.get_include()


ext_modules = [
    Extension(
        "client",
        ["integration_client.cpp", "client_bindings.cpp"],
        include_dirs=[get_pybind_include()],
        language="c++",
        extra_compile_args=["-std=c++11"],
    ),
]

setup(
    name="client",
    version="1.0",
    description="Client module for Webserv integration test",
    ext_modules=ext_modules,
    install_requires=["pybind11>=2.5.0"],
    cmdclass={"build_ext": build_ext},
    zip_safe=False,
)

# python setup.py build_ext --inplace
