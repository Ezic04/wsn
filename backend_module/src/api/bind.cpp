#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include "api/SimulationManager.hpp"
#include "core/Simulation.hpp"
#include "core/Sensor.hpp"
/**
 * @file bind.cpp
 * @brief Binds C++ classes and functions to Python using pybind11.
 */

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
        .def(py::init<double, uint32_t, uint32_t, SimulationStopCondition, float, uint32_t>(),
             py::arg("sensor_radious"),
             py::arg("initial_battery_lvl"),
             py::arg("reshuffle_interval"),
             py::arg("stop_condition"),
             py::arg("stop_threshold"),
             py::arg("max_ticks"))
        .def_readwrite("sensor_radious", &SimulationParameters::sensor_radius)
        .def_readwrite("initial_battery_lvl", &SimulationParameters::initial_battery_lvl)
        .def_readwrite("reshuffle_interval", &SimulationParameters::reshuffle_interval);

    py::class_<SimulationScenario>(m, "SimulationScenario")
        .def(py::init<>())
        .def(py::init<std::vector<Point>, std::vector<Point>>(),
             py::arg("target_positions"),
             py::arg("sensor_positions"))
        .def_readwrite("target_positions", &SimulationScenario::target_positions)
        .def_readwrite("sensor_positions", &SimulationScenario::sensor_positions);

    py::class_<SimulationState>(m, "SimulationState")
        .def_readwrite("tick", &SimulationState::tick)
        .def_readwrite("all_target_covered", &SimulationState::all_target_covered)
        .def_readwrite("covered_target_count", &SimulationState::covered_target_count)
        .def_readwrite("coverage_percentage", &SimulationState::coverage_percentage)
        .def_readwrite("is_target_covered", &SimulationState::is_target_covered)
        .def_readwrite("sensor_states", &SimulationState::sensor_states)
        .def_readwrite("sensor_battery_lvls", &SimulationState::sensor_battery_lvls);

    py::enum_<SimulationStopCondition>(m, "SimulationStopCondition")
        .value("kManual", SimulationStopCondition::kManual)
        .value("kZeroCoverage", SimulationStopCondition::kZeroCoverage)
        .value("kCoverageBelowThreshold", SimulationStopCondition::kCoverageBelowThreshold)
        .value("kAnyCoverageLost", SimulationStopCondition::kAnyCoverageLost);

    py::class_<SimulationManager>(m, "SimulationManager")
        .def(py::init<>())
        .def("GetSimulationStates", &SimulationManager::GetSimulationStates, py::return_value_policy::reference)
        .def("GetParameters", &SimulationManager::GetParameters, py::return_value_policy::reference)
        .def("GetScenario", &SimulationManager::GetScenario, py::return_value_policy::reference)
        .def("IsInitialized", &SimulationManager::IsInitialized)
        .def("LoadParametersFromJSON", &SimulationManager::LoadParametersFromJSON)
        .def("LoadScenarioFromJSON", &SimulationManager::LoadScenarioFromJSON)
        .def("SetParameters", &SimulationManager::SetParameters)
        .def("SetScenario", &SimulationManager::SetScenario)
        .def("Initialize", &SimulationManager::Initialize)
        .def("Run", &SimulationManager::Run)
        .def("Reset", &SimulationManager::Reset);

    py::bind_vector<std::vector<bool>>(m, "VectorBool");
    py::bind_vector<std::vector<int32_t>>(m, "VectorInt32");
    py::bind_vector<std::vector<Sensor::State>>(m, "VectorSensorState");
    py::bind_vector<std::vector<SimulationState>>(m, "VectorSimulationState");
}