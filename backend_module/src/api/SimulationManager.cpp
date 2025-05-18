#include "api/SimulationManager.hpp"
#define RD 0
using json = nlohmann::json;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationScenario, sensor_positions, target_positions)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationParameters, sensor_radious, initial_battery_lvl, reshuffle_interval)
// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationState, tick, all_target_covered, covered_target_count, is_target_covered, sensor_states, sensor_battery_lvls)

void SimulationManager::SetStopCondition(StopCondition condition, float threshold)
{
  condition_ = condition;
  stop_threshold_ = threshold;
}

const SimulationParameters &SimulationManager::GetParameters() const
{
  if (!parameters_.has_value())
  {
    throw std::runtime_error("Parameters not set");
  }
  return parameters_.value();
}

const SimulationScenario &SimulationManager::GetScenario() const
{
  if (!scenario_.has_value())
  {
    throw std::runtime_error("Scenario not set");
  }
  return scenario_.value();
}

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

void SimulationManager::LoadRandomScenario(uint32_t target_num, uint32_t sensor_num)
{
  scenario_ = SimulationScenario();
  auto &target_positions = scenario_->target_positions;
  auto &sensor_positions = scenario_->sensor_positions;
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
  if (!parameters_.has_value())
  {
    throw std::runtime_error("Parameters not set");
  }
  if (!scenario_.has_value())
  {
    throw std::runtime_error("Scenario not set");
  }
  if (simulation_.has_value())
  {
    throw std::runtime_error("Simulation already initialized");
  }
  simulation_ = Simulation();
  simulation_->Initialize(*parameters_, *scenario_);
}

void SimulationManager::Run(uint32_t max_ticks)
{
  if (!simulation_.has_value())
  {
    throw std::runtime_error("Simulation not initialized");
  }
  for (uint32_t i = 0; i < max_ticks; ++i)
  {
    simulation_->Tick();
    SimulationState state = simulation_->GetSimulationState();
    states_.emplace_back(state);
    if (ShouldStop(state))
    {
      break;
    }
  }
}

void SimulationManager::Reset()
{
  simulation_.reset();
  states_.clear();
}

bool SimulationManager::ShouldStop(SimulationState state)
{
  switch (condition_)
  {
  case StopCondition::kManual:
    return false;
    break;
  case StopCondition::kZeroCoverage:
    return !state.all_target_covered;
    break;
  case StopCondition::kCoverageBelowThreshold:
    return state.coverage_percentage < stop_threshold_;
    break;
  case StopCondition::kAnyCoverageLost:
    return state.covered_target_count == 0;
    break;
  default:
    return false;
    break;
  }
}
