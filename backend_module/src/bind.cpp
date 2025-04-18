#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <WSNManager.hpp>
#include <Simulation.hpp>
#include <Sensor.hpp>

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<Target>);
PYBIND11_MAKE_OPAQUE(std::vector<Sensor>);


PYBIND11_MODULE(backend_module, m)
{ 
    py::class_<Point>(m, "Point")
    .def(py::init<>())
    .def(py::init<float, float>())
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y)
    .def("__mul__", &Point::operator*)
    .def("__imul__", &Point::operator*=);


    py::class_<Target>(m, "Target")
    .def(py::init<Point>())    
    .def_property_readonly("position", &Target::GetPosition);
    
    py::class_<Sensor>(m, "Sensor")
    .def(py::init<Point, uint32_t>())
    .def_property_readonly_static("Radius", [](py::object /* cls */) { return Sensor::GetRadius(); })
    .def_property_readonly("position", &Sensor::GetPosition);
    
    py::bind_vector<std::vector<Target>>(m, "VectorTarget");
    py::bind_vector<std::vector<Sensor>>(m, "VectorSensor");
    
    py::class_<Simulation>(m, "Simulation")
    .def(py::init<>())
    .def("Initialization", &Simulation::Initialization, 
        py::arg("target_num"), py::arg("sensor_num"), py::arg("sensor_radious"))
        .def_readonly("Targets", &Simulation::targets_)
        .def_readonly("Sensors", &Simulation::sensors_)
        .def("RunSimulation", &Simulation::RunSimulation);

};