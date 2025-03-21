#include <pybind11/pybind11.h>
#include <bind.hpp>
// Example function that we want to expose in Python.


PYBIND11_MODULE(backend, m) {
    // Bind the C++ functions so they can be used from Python.
    m.def("add", &add, "A function which adds two numbers");
}