#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "core/Simulation.hpp"
#include "shared/utility.hpp"
#include "shared/data_structures.hpp"
#include "api/json.hpp"

class SimulationManager
{
public:
  enum class StopCondition
  {
    kManual,
    kZeroCoverage,
    kCoverageBelowThreshold,
    kAnyCoverageLost
  };

private:
  std::optional<SimulationParameters> parameters_;
  std::optional<SimulationScenario> scenario_;
  std::optional<Simulation> simulation_;
  std::vector<SimulationState> states_;
  StopCondition condition_;
  float stop_threshold_;

public:
  void SetStopCondition(StopCondition condition, float threshold = 0.9f);
  const std::vector<SimulationState> &GetSimulationStates() const { return states_; }
  const SimulationParameters &GetParameters() const;
  const SimulationScenario &GetScenario() const;
  void LoadFromJSON(const std::string &json_path);
  void LoadParameters(const SimulationParameters &paramseters) { parameters_ = paramseters; }
  void LoadScenario(const SimulationScenario &scenario) { scenario_ = scenario; }
  void LoadRandomScenario(uint32_t target_num, uint32_t sensor_num);
  void Initialize();
  void Run(uint32_t max_ticks);
  void Reset();

private:
  bool ShouldStop(SimulationState state);
};