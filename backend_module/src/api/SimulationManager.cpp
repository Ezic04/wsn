#include "api/SimulationManager.hpp"
#include <fstream>
#include "api/json.hpp"
#include "shared/data_structures.hpp"

#define RD 0

using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationScenario, sensor_positions, target_positions)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationParameters, sensor_radious, initial_battery_lvl, reshuffle_interval)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationState, tick, all_target_covered, covered_target_count, is_target_covered, sensor_states, sensor_battery_lvls)

void SimulationManager::LoadFromJSON(const std::string &json_path)
{
  std::ifstream file(json_path);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open JSON file: " + json_path);
  }
  json j;
  file >> j;
  if (!j.contains("scenario") || !j.contains("parameters"))
  {
    throw std::runtime_error("Invalid JSON format: missing 'scenario' or 'parameters'");
  }
  scenario_ = j.at("scenario").get<SimulationScenario>();
  parameters_ = j.at("parameters").get<SimulationParameters>();
}

void SimulationManager::LoadRandomScenario(uint16_t target_num, uint16_t sensor_num)
{
  auto &target_positions = scenario_.target_positions;
  auto &sensor_positions = scenario_.sensor_positions;
  std::random_device rd;
#ifdef RD
  std::mt19937 gen(RD);
#else
  std::mt19937 gen(rd());
#endif
  std::uniform_real_distribution<> dist(0.0, 1.0);
  sensor_positions.reserve(sensor_num);
  target_positions.reserve(target_num);
  for (size_t i = 0; i < sensor_num; ++i)
  {
    sensor_positions.emplace_back(dist(gen), dist(gen));
  }
  for (size_t i = 0; i < target_num; ++i)
  {
    target_positions.emplace_back(dist(gen), dist(gen));
  }
}

void SimulationManager::Initialize()
{
  simulation_.Initialize(parameters_, scenario_);
}

void SimulationManager::Tick()
{
  simulation_.Tick();
  state_ = simulation_.GetSimulationState();
}