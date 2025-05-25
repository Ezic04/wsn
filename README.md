# Wireless Sensor Network Simulation

This project implements a simulation of a wireless sensor network (WSN) in C++, with a Python graphical interface using Tkinter and PIL. The simulation models sensors, targets, and their interactions, and is visualized in a step-by-step or automated fashion. The backend is built in C++ with `pybind11` bindings, and the frontend is a Python application.

## Features

- Simulation of sensor network coverage
- Supports sensor battery levels and state transitions (on, off, dead)
- Visualization of the simulation: sensors, targets, coverage areas
- Step-by-step and automatic playback modes in the UI
- Configurable parameters and scenarios via JSON files

## Project Structure
.
├── backend_module/
│ ├── include/
│ │ ├── api/
│ │ ├── core/
│ │ └── shared/
│ ├── src/
│ │ ├── api/
│ │ ├── core/
│ │ └── test.cpp
│ ├── build/
│ ├── CMakeLists.txt
├── example_configs/
│ ├── config1.json
│ ├── config2.json
│ ├── ...
├── lib/
│ └── backend_module/
│ ├── init.py
│ ├── backend_module.pyd
│ └── backend_module.pyi
├── user_interface/
│ ├── Interface.py
│ ├── cpp_tests.py
│ └── main.py
├── CMakeLists.txt
├── build.py
├── requirements.txt
└── README.md

## Dependencies

### C++ Libraries

- `nlohmann::json`
- `pybind11==2.13.6`

### Python Libraries

- `pillow==11.1.0`
- `pybind11-stubgen==2.5.3`

### Tools

- CMake (`>=4.0.0-rc4`)
- C++ compiler (`g++ 14.2.0`)
- Python (`>=3.13.2`)
- VSCode (with recommended extensions)

## Recommended VSCode Extensions

- C++
- Python
- Python Debugger
- Pylance
- CMake

## Setup Instructions

1. Clone the repository.

2. Run build.py

Ensure the correct Python interpreter is selected in VSCode:
- Use the `.venv` created by `build.py`.
- The terminal should show `(.venv)` in the prompt.
- If issues occur, restart VSCode.

python user_interface/Interface.py

5. Run the simulation UI:


## JSON Configuration

### Example Format

`example_configs/config1.json`:
```json
{
  "parameters": {
    "sensor_radious": 0.5,
    "initial_battery_lvl": 64,
    "reshuffle_interval": 8,
    "max_ticks": 1024,
    "stop_condition": "kZeroCoverage",
    "stop_threshold": 0.0
  },
  "scenario": {
    "sensor_positions": [
      { "x": 0.4, "y": 0.4 },
      { "x": 0.6, "y": 0.6 }
    ],
    "target_positions": [
      { "x": 0.5, "y": 0.5 }
    ]
  }
}
```

- `sensor_radious`: Sensor detection range (normalized).
- `initial_battery_lvl`: Starting battery level for all sensors.
- `reshuffle_interval`: Reshuffle period (ticks).
- `max_ticks`: Maximum simulation steps.
- `stop_condition`: One of:
  - `"kManual"`
  - `"kZeroCoverage"`
  - `"kCoverageBelowThreshold"`
  - `"kAnyCoverageLost"`
- `stop_threshold`: Minimum required coverage (if `kCoverageBelowThreshold` is used).

## Running the Simulation

- Load parameters and scenario from a JSON file via the UI.
- Run the simulation in either step-by-step or automatic mode.
- Visualize:
  - Sensors (with color-coded state and coverage radius)
  - Targets (covered/uncovered status)
  - Current simulation tick and coverage percentage

## Notes
