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
  std::optional<SimulationParameters> parameters_;
  std::optional<SimulationScenario> scenario_;
  std::optional<Simulation> simulation_;
  std::vector<SimulationState> states_;

public:
  // void SetStopCondition(SimulationStopCondition condition, float threshold = 0.9f);
  const std::vector<SimulationState> &GetSimulationStates() const { return states_; }
  const SimulationParameters &GetParameters() const;
  const SimulationScenario &GetScenario() const;
  void LoadFromJSON(const std::string &json_path);
  void LoadParameters(const SimulationParameters &paramseters);
  void LoadScenario(const SimulationScenario &scenario);
  void LoadRandomScenario(uint32_t target_num, uint32_t sensor_num);
  void Initialize();
  void Run();
  void Reset();

private:
  bool ShouldStop(SimulationState state);
};