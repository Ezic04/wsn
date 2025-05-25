#include "api/SimulationManager.hpp"

#define RD 0

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

void SimulationManager::SetParameters(const SimulationParameters &parameters)
{
  if (parameters_.has_value() && is_initialized_)
  {
    throw std::runtime_error("Cannot set parameters after initialization");
  }
  if(parameters.sensor_radious <= 0)
  {
    throw std::runtime_error("Sensor radius must be positive");
  }
  if (parameters.initial_battery_lvl < parameters.reshuffle_interval)
  {
    throw std::runtime_error("Initial battery level must be greater than reshuffle interval");
  }
  if (parameters.max_ticks == 0)
  {
    throw std::runtime_error("Max ticks must be greater than 0");
  }
  if (parameters.stop_condition == SimulationStopCondition::kCoverageBelowThreshold && parameters.stop_threshold <= 0)
  {
    throw std::runtime_error("Stop threshold must be greater than 0");
  }
  parameters_ = parameters;
}

void SimulationManager::SetScenario(const SimulationScenario &scenario)
{
  if (scenario_.has_value() && is_initialized_)
  {
    throw std::runtime_error("Cannot set scenario after initialization");
  }
  if (scenario.target_positions.empty())
  {
    throw std::runtime_error("Scenario must contain at least one target");
  }
  if (scenario.sensor_positions.empty())
  {
    throw std::runtime_error("Scenario must contain at least one sensor");
  }
  scenario_ = scenario;
}

void SimulationManager::LoadParametersFromJSON(const std::string &json_path)
{
  auto j = LoadJSON(json_path);
  if (!j.contains("parameters"))
  {
    throw std::runtime_error("JSON does not contain 'parameters' field.");
  }
  parameters_ = j.at("parameters").get<SimulationParameters>();
}

void SimulationManager::LoadScenarioFromJSON(const std::string &json_path)
{
  auto j = LoadJSON(json_path);
  if (!j.contains("scenario"))
  {
    throw std::runtime_error("JSON does not contain 'scenario' field.");
  }
  scenario_ = j.at("scenario").get<SimulationScenario>();
}

void SimulationManager::DumpStatesToJSON(const std::string& json_path) const
{
  if (states_.empty())
  {
    throw std::runtime_error("No simulation states to dump");
  }
  nlohmann::json j = states_;
  std::ofstream file(json_path);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open file for writing: " + json_path);
  }
  file << std::setw(2) << j << '\n';
}

// void SimulationManager::LoadRandomScenario(uint32_t target_num, uint32_t sensor_num)
// {
//   scenario_ = SimulationScenario();
//   auto &target_positions = scenario_->target_positions;
//   auto &sensor_positions = scenario_->sensor_positions;
//   std::random_device rd;
// #ifdef RD
//   std::mt19937 gen(RD);
// #else
//   std::mt19937 gen(rd());
// #endif
//   std::uniform_real_distribution<> dist(0.0, 1.0);
//   sensor_positions.reserve(sensor_num);
//   target_positions.reserve(target_num);
//   for (size_t i = 0; i < sensor_num; ++i)
//   {
//     sensor_positions.emplace_back(dist(gen), dist(gen));
//   }
//   for (size_t i = 0; i < target_num; ++i)
//   {
//     target_positions.emplace_back(dist(gen), dist(gen));
//   }
// }

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
  is_initialized_ = true;
}

void SimulationManager::Run()
{
  if (!simulation_.has_value())
  {
    throw std::runtime_error("Simulation not initialized");
  }
  auto max_ticks = parameters_->max_ticks;
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
  parameters_.reset();
  scenario_.reset();
  simulation_.reset();
  states_.clear();
  is_initialized_ = false;
}

json SimulationManager::LoadJSON(const std::string &json_path)
{
  std::ifstream file(json_path);
  if (!file.is_open())
  {
    throw std::runtime_error("Failed to open JSON file: " + json_path);
  }
  json j;
  file >> j;
  return j;
}

bool SimulationManager::ShouldStop(const SimulationState &state)
{
  switch (parameters_->stop_condition)
  {
  case SimulationStopCondition::kManual:
    return false;
    break;
  case SimulationStopCondition::kZeroCoverage:
    return state.covered_target_count == 0;
    break;
  case SimulationStopCondition::kCoverageBelowThreshold:
    return state.coverage_percentage < parameters_->stop_threshold;
    break;
  case SimulationStopCondition::kAnyCoverageLost:
    return !state.all_target_covered;
    break;
  default:
    return false;
    break;
  }
}