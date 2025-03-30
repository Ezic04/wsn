#include <pybind11/pybind11.h>
#include <WSNManager.hpp>
#include <Simulation.hpp>
#include <Sensor.hpp>

namespace py = pybind11;

PYBIND11_MODULE(backend_module, m)
{
    py::class_<Point>(m, "Point")
        .def(py::init<>())
        .def(py::init<float, float>())
        .def_readwrite("x", &Point::x)
        .def_readwrite("y", &Point::y);

    py::class_<Sensor>(m, "Sensor")
        .def(py::init<Point>())
        .def_static("SetRadious", &Sensor::SetRadius)
        .def("GetPosition", &Sensor::GetPosition);

    py::class_<Simulation>(m, "Simulation")
        .def(py::init<>())
        .def("Initialization", &Simulation::Initialization, 
            py::arg("target_num"), py::arg("sensor_num"), py::arg("sensor_radious"));
}