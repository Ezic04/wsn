#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "api/SimulationManager.hpp"
#include "core/Simulation.hpp"
#include "core/Sensor.hpp"

namespace py = pybind11;

// PYBIND11_MAKE_OPAQUE(std::vector<Target>);
// PYBIND11_MAKE_OPAQUE(std::vector<Sensor>);


// PYBIND11_MODULE(backend_module, m)
// { 
//     py::class_<Point>(m, "Point")
//     .def(py::init<>())
//     .def(py::init<float, float>())
//     .def_readwrite("x", &Point::x)
//     .def_readwrite("y", &Point::y)
//     .def("__mul__", &Point::operator*)
//     .def("__imul__", &Point::operator*=);


//     py::class_<Target>(m, "Target")
//     .def(py::init<Point>())    
//     .def_property_readonly("position", &Target::GetPosition);
    
//     py::class_<Sensor>(m, "Sensor")
//     .def(py::init<Point, uint32_t>())
//     .def_property_readonly_static("Radius", [](py::object /* cls */) { return Sensor::GetRadius(); })
//     .def_property_readonly("position", &Sensor::GetPosition);
    
//     py::bind_vector<std::vector<Target>>(m, "VectorTarget");
//     py::bind_vector<std::vector<Sensor>>(m, "VectorSensor");
    
//     py::class_<Simulation>(m, "Simulation")
//     .def(py::init<>())
//     .def("Initialization", &Simulation::Initialize, 
//         py::arg("target_num"), py::arg("sensor_num"), py::arg("sensor_radious"))
//         .def_readonly("Targets", &Simulation::targets_)
//         .def_readonly("Sensors", &Simulation::sensors_)
//         .def("RunSimulation", &Simulation::RunSimulation);

// };

namespace py = pybind11;

PYBIND11_MAKE_OPAQUE(std::vector<bool>);
PYBIND11_MAKE_OPAQUE(std::vector<uint32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<int32_t>);
PYBIND11_MAKE_OPAQUE(std::vector<Sensor::State>);
PYBIND11_MAKE_OPAQUE(std::vector<SimulationState>);

PYBIND11_MODULE(backend_module, m)
{
  py::class_<Point>(m, "Point")
    .def(py::init<>())
    .def(py::init<double, double>())
    .def_readwrite("x", &Point::x)
    .def_readwrite("y", &Point::y)
    .def("__mul__", &Point::operator*)
    .def("__imul__", &Point::operator*=);

  py::enum_<Sensor::State>(m, "SensorState")
    .value("kOn", Sensor::State::kOn)
    .value("kOff", Sensor::State::kOff)
    .value("kDead", Sensor::State::kDead)
    .value("kUndecided", Sensor::State::kUndecided);

  py::class_<SimulationParameters>(m, "SimulationParameters")
    .def(py::init<>())
    .def_readwrite("sensor_radious", &SimulationParameters::sensor_radious)
    .def_readwrite("initial_battery_lvl", &SimulationParameters::initial_battery_lvl)
    .def_readwrite("reshuffle_interval", &SimulationParameters::reshuffle_interval);

  py::class_<SimulationScenario>(m, "SimulationScenario")
    .def(py::init<>())
    .def_readwrite("target_positions", &SimulationScenario::target_positions)
    .def_readwrite("sensor_positions", &SimulationScenario::sensor_positions);

  py::class_<SimulationState>(m, "SimulationState")
    .def(py::init<>())
    .def_readwrite("tick", &SimulationState::tick)
    .def_readwrite("all_target_covered", &SimulationState::all_target_covered)
    .def_readwrite("covered_target_count", &SimulationState::covered_target_count)
    .def_readwrite("coverage_percentage", &SimulationState::coverage_percentage)
    .def_readwrite("is_target_covered", &SimulationState::is_target_covered)
    .def_readwrite("sensor_states", &SimulationState::sensor_states)
    .def_readwrite("sensor_battery_lvls", &SimulationState::sensor_battery_lvls);

  py::enum_<SimulationManager::StopCondition>(m, "StopCondition")
    .value("kManual", SimulationManager::StopCondition::kManual)
    .value("kZeroCoverage", SimulationManager::StopCondition::kZeroCoverage)
    .value("kCoverageBelowThreshold", SimulationManager::StopCondition::kCoverageBelowThreshold)
    .value("kAnyCoverageLost", SimulationManager::StopCondition::kAnyCoverageLost);

  py::class_<SimulationManager>(m, "SimulationManager")
    .def(py::init<>())
    .def("SetStopCondition", &SimulationManager::SetStopCondition)
    .def("GetSimulationStates", &SimulationManager::GetSimulationStates, py::return_value_policy::reference)
    .def("GetParameters", &SimulationManager::GetParameters, py::return_value_policy::reference)
    .def("GetScenario", &SimulationManager::GetScenario, py::return_value_policy::reference)
    .def("LoadFromJSON", &SimulationManager::LoadFromJSON)
    .def("LoadParameters", &SimulationManager::LoadParameters)
    .def("LoadScenario", &SimulationManager::LoadScenario)
    .def("LoadRandomScenario", &SimulationManager::LoadRandomScenario)
    .def("Initialize", &SimulationManager::Initialize)
    .def("Run", &SimulationManager::Run)
    .def("Reset", &SimulationManager::Reset);

  py::bind_vector<std::vector<bool>>(m, "VectorBool");
  py::bind_vector<std::vector<int32_t>>(m, "VectorInt32");
  py::bind_vector<std::vector<Sensor::State>>(m, "VectorSensorState");
  py::bind_vector<std::vector<SimulationState>>(m, "VectorSimulationState");
}