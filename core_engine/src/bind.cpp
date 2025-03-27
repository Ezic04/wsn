#include <pybind11/pybind11.h>
#include <bind.hpp>
#include <Sensor.hpp>

namespace py = pybind11;

PYBIND11_MODULE(core_engine, m)
{
    m.def("add", &add, "A function which adds two numbers");

    py::class_<Point>(m, "Point")
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y);

    py::class_<Sensor>(m, "Sensor")
        .def(py::init<Point>())
        .def_readwrite_static("Radius", &Sensor::Radius)
        .def_static("SetRadious", &Sensor::SetRadius);
}