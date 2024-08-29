#include "integration_client.hpp"
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(client, m) {
	py::class_<Client>(m, "Client")
		.def(py::init<unsigned int>())
		.def("SendRequestAndReceiveResponse", &Client::SendRequestAndReceiveResponse);
}
