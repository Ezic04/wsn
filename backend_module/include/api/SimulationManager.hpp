#pragma once
#include <vector>
#include "api/json.hpp"
#include "core/Simulation.hpp"
#include "shared/data_structures.hpp"

enum class StopCondition
{
  kAnyCoverageLost,
  kCoverageBelowThreshold,
  kZeroCoverage,
  kManual
};

class SimulationManager
{
  SimulationParameters parameters_;
  SimulationScenario scenario_;
  SimulationState state_;
  Simulation simulation_;

public:
  SimulationManager() = default;
  SimulationState GetCurrentState() const { return state_; }
  const SimulationParameters &GetParameters() const { return parameters_; }
  const SimulationScenario &GetScenario() const { return scenario_; }
  void LoadFromJSON(const std::string &json_path);
  void LoadParameters(const SimulationParameters &paramseters) { parameters_ = paramseters; }
  void LoadScenario(const SimulationScenario &scenario) { scenario_ = scenario; }
  void LoadRandomScenario(uint16_t target_num, uint16_t sensor_num);
  void Initialize();
  void Tick();
  // void Reset();
  // // std::vector<SimulationState> RunAndCollect();

  // // Export
  // // std::string DumpStateAsJSON() const;
};
