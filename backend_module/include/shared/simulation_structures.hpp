#pragma once
#include <ostream>
#include <cstdint>
#include <vector>
#include "core/Sensor.hpp"
#include "shared/utility.hpp"
#include "api/json.hpp"
/**
 * @file simulation_structures.hpp
 * @brief Defines structures and types used in the simulation of sensor networks.
 */

using json = nlohmann::json;

enum class SimulationStopCondition ///< The condition that determines when the simulation stops.
{
  kManual,
  kZeroCoverage,
  kCoverageBelowThreshold,
  kAnyCoverageLost
};

/**
 * @struct SimulationParameters
 * @brief Contains parameters for the simulation.
 * @details This structure holds the sensor radius, initial battery level, reshuffle interval, maximum ticks, stop condition, and stop threshold.
 */
struct SimulationParameters
{
  double sensor_radius;                   ///< The sensing radius of the sensors in the simulation.
  uint32_t initial_battery_lvl;           ///< The initial battery level of the sensors in the simulation.
  uint32_t reshuffle_interval;            ///< The interval at which sensors are reshuffled in the simulation.
  uint32_t max_ticks;                     ///< The maximum number of ticks for the simulation to run.
  SimulationStopCondition stop_condition; ///< The condition that determines when the simulation stops.
  float stop_threshold;                   ///< The threshold for stopping the simulation, applicable based on the stop condition.

  SimulationParameters() = default;

  SimulationParameters(double sensor_radius,
                       uint32_t initial_battery_lvl,
                       uint32_t reshuffle_interval,
                       SimulationStopCondition condition,
                       float stop_threshold,
                       uint32_t max_ticks)
      : sensor_radius(sensor_radius),
        initial_battery_lvl(initial_battery_lvl),
        reshuffle_interval(reshuffle_interval),
        max_ticks(max_ticks),
        stop_condition(condition),
        stop_threshold(stop_threshold) {}
};

/**
 * @struct SimulationScenario
 * @brief Represents a scenario for the simulation.
 * @details This structure contains the positions of targets and sensors in the simulation.
 */
struct SimulationScenario
{
  std::vector<Point> target_positions; ///< The positions of targets in the simulation.
  std::vector<Point> sensor_positions; ///< The positions of sensors in the simulation.

  SimulationScenario() = default;

  SimulationScenario(std::vector<Point> target_positions,
                     std::vector<Point> sensor_positions)
      : target_positions(target_positions),
        sensor_positions(sensor_positions) {}
};


/**
 * @struct SimulationState
 * @brief Represents the state of the simulation at a given tick.
 * @details This structure contains information about the current tick, coverage status, sensor states, and battery levels.
 */
struct SimulationState
{
  uint32_t tick;                            ///< The current tick of the simulation.
  bool all_target_covered;                  ///< Indicates if all targets are covered by sensors.
  uint32_t covered_target_count;            ///< The count of targets that are currently covered by sensors.
  float coverage_percentage;                ///< The percentage of coverage achieved by the sensors. Number between 0 and 1.
  std::vector<bool> is_target_covered;      ///< A vector indicating whether each target is covered by at least one sensor.
  std::vector<Sensor::State> sensor_states; ///< A vector containing the states of each sensor in the simulation.
  std::vector<int32_t> sensor_battery_lvls; ///< A vector containing the battery levels of each sensor in the simulation.

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
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationParameters, sensor_radius, initial_battery_lvl, reshuffle_interval, max_ticks, stop_condition, stop_threshold)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SimulationScenario, sensor_positions, target_positions)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(
    SimulationState,
    tick,
    all_target_covered,
    covered_target_count,
    coverage_percentage,
    is_target_covered,
    sensor_states,
    sensor_battery_lvls)

NLOHMANN_JSON_SERIALIZE_ENUM(
    Sensor::State,
    {
        {Sensor::State::kOn, "kOn"},
        {Sensor::State::kOff, "kOff"},
        {Sensor::State::kDead, "kDead"},
        {Sensor::State::kUndecided, "kUndecided"},
    })

NLOHMANN_JSON_SERIALIZE_ENUM(
    SimulationStopCondition,
    {
        {SimulationStopCondition::kManual, "kManual"},
        {SimulationStopCondition::kZeroCoverage, "kZeroCoverage"},
        {SimulationStopCondition::kCoverageBelowThreshold, "kCoverageBelowThreshold"},
        {SimulationStopCondition::kAnyCoverageLost, "kAnyCoverageLost"},
    })
