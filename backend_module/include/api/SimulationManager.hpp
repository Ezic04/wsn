#pragma once
#include <vector>
#include <fstream>
#include <optional>
#include "core/Simulation.hpp"
#include "shared/utility.hpp"
#include "shared/simulation_structures.hpp"
#include "api/json.hpp"

class SimulationManager
{
  std::optional<SimulationParameters> parameters_;
  std::optional<SimulationScenario> scenario_;
  std::optional<Simulation> simulation_;
  std::vector<SimulationState> states_;
  bool is_initialized_ = false;

public:
  const SimulationParameters &GetParameters() const;
  const SimulationScenario &GetScenario() const;
  const std::vector<SimulationState> &GetSimulationStates() const { return states_; }
  bool IsInitialized() const { return is_initialized_; }
  void SetParameters(const SimulationParameters &paramseters);
  void SetScenario(const SimulationScenario &scenario);
  void LoadParametersFromJSON(const std::string &json_path);
  void LoadScenarioFromJSON(const std::string &json_path);
  // void DumpStatesToJSON(const std::string& json_path) const;
  // void LoadRandomScenario(uint32_t target_num, uint32_t sensor_num);
  void Initialize();
  void Run();
  void Reset();

private:
  json LoadJSON(const std::string &json_path);
  bool ShouldStop(const SimulationState &state);
};