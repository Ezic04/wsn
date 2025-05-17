#pragma once
#include <ostream>
#include <cstdint>
#include <vector>
#include "core/Sensor.hpp"
#include "shared/utility.hpp"

struct SimulationParameters
{
  double sensor_radious;
  uint32_t initial_battery_lvl;
  uint32_t reshuffle_interval;
};

struct SimulationScenario
{
  std::vector<Point> target_positions;
  std::vector<Point> sensor_positions;
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
};