#pragma once
#include <ostream>
#include <cstdint>
#include <vector>
#include "core/Sensor.hpp"
#include "shared/utility.hpp"
#include "api/json.hpp"

using json = nlohmann::json;


enum class SimulationStopCondition
{
  kManual,
  kZeroCoverage,
  kCoverageBelowThreshold,
  kAnyCoverageLost
};

struct SimulationParameters
{
  double sensor_radious;
  uint32_t initial_battery_lvl;
  uint32_t reshuffle_interval;
  uint32_t max_ticks;
  SimulationStopCondition stop_condition;
  float stop_threshold;

  SimulationParameters() = default;

  SimulationParameters(double sensor_radious,
                       uint32_t initial_battery_lvl,
                       uint32_t reshuffle_interval,
                       SimulationStopCondition condition,
                       float stop_threshold,
                       uint32_t max_ticks)
      : sensor_radious(sensor_radious),
        initial_battery_lvl(initial_battery_lvl),
        reshuffle_interval(reshuffle_interval),
        max_ticks(max_ticks),
        stop_condition(condition),
        stop_threshold(stop_threshold) {}
};

struct SimulationScenario
{
  std::vector<Point> target_positions;
  std::vector<Point> sensor_positions;

  SimulationScenario() = default;

  SimulationScenario(std::vector<Point> target_positions,
                     std::vector<Point> sensor_positions)
      : target_positions(target_positions),
        sensor_positions(sensor_positions) {}
};

struct SimulationState
{
  uint32_t tick;
  bool all_target_covered;
  uint32_t covered_target_count;
  float coverage_percentage;
  std::vector<bool> is_target_covered;
  std::vector<Sensor::State> sensor_states;
  std::vector<int32_t> sensor_battery_lvls;

  SimulationState() = default;

  SimulationState(uint32_t tick,
                  bool all_target_covered,
                  uint32_t covered_target_count,
                  float coverage_percentage,
                  std::vector<bool> is_target_covered,
                  std::vector<Sensor::State> sensor_states,
                  std::vector<int32_t> sensor_battery_lvls)
      : tick(tick),
        all_target_covered(all_target_covered),
        covered_target_count(covered_target_count),
        coverage_percentage(coverage_percentage),
        is_target_covered(is_target_covered),
        sensor_states(sensor_states),
        sensor_battery_lvls(sensor_battery_lvls) {}
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Point, x, y)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationParameters, sensor_radious, initial_battery_lvl, reshuffle_interval, max_ticks, stop_condition, stop_threshold)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationScenario, sensor_positions, target_positions)
// NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationState, tick, all_target_covered, covered_target_count, is_target_covered, sensor_states, sensor_battery_lvls)

inline void to_json(nlohmann::json &j, const SimulationStopCondition &cond)
{
  switch (cond)
  {
  case SimulationStopCondition::kManual:
    j = "kManual";
    break;
  case SimulationStopCondition::kZeroCoverage:
    j = "kZeroCoverage";
    break;
  case SimulationStopCondition::kCoverageBelowThreshold:
    j = "kCoverageBelowThreshold";
    break;
  case SimulationStopCondition::kAnyCoverageLost:
    j = "kAnyCoverageLost";
    break;
  }
}

inline void from_json(const nlohmann::json &j, SimulationStopCondition &cond)
{
  const std::string s = j.get<std::string>();
  if (s == "kManual")
    cond = SimulationStopCondition::kManual;
  else if (s == "kZeroCoverage")
    cond = SimulationStopCondition::kZeroCoverage;
  else if (s == "kCoverageBelowThreshold")
    cond = SimulationStopCondition::kCoverageBelowThreshold;
  else if (s == "kAnyCoverageLost")
    cond = SimulationStopCondition::kAnyCoverageLost;
  else
    throw std::invalid_argument("Invalid stop_condition value: " + s);
}
