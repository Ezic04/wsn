#pragma once
#include <vector>
#include <algorithm>
#include <random>
#include <stdexcept>
// #include <iostream> //for debug

#include "core/Sensor.hpp"
#include "shared/utility.hpp"
#include "shared/simulation_structures.hpp"
/**
 * @file Simulation.hpp
 * @brief Contains the Simulation class that manages the simulation of sensor networks.
 */

/**
 * @class Simulation
 * @brief Manages the simulation of sensor networks.
 * @details This class initializes and runs the simulation, managing the state of sensors and targets.
 */
class Simulation
{
  uint32_t reshuffle_interval_;    ///< The interval at which sensors are reshuffled in the simulation.
  uint32_t initial_battery_lvl_;   ///< The initial battery level of the sensors in the simulation.
  uint32_t tick_;                  ///< The current tick of the simulation.
  uint32_t covered_targets_count_; ///< The count of targets that are currently covered by sensors.
  bool all_target_covered_;        ///< Indicates if all targets are covered by sensors.
  std::vector<Target> targets_;    ///< List of targets in the simulation.
  std::vector<Sensor> sensors_;    ///< List of sensors in the simulation.
  size_t target_num;               ///< The number of targets in the simulation.
  size_t sensor_num;               ///< The number of sensors in the simulation.

public:
  Simulation() : tick_(-1), all_target_covered_(false), covered_targets_count_(0) {} ///< Default constructor initializes the simulation with default values.
  /**
   * @brief Constructs a Simulation with given parameters and scenario.
   * @param parameters The simulation parameters.
   * @param scenario The simulation scenario containing target and sensor positions.
   */
  void Initialize(const SimulationParameters &parameters, const SimulationScenario &scenario);
  /**
   * @brief Gets the current state of the simulation.
   * @return A SimulationState object containing the current state of the simulation.
   */
  SimulationState GetSimulationState();
  /**
   * @brief Advances the simulation by one tick.
   * @details This method updates the state of sensors and targets, reshuffles sensors if necessary, and counts coverage.
   * @note Simulation must be initialized before calling this method.
   */
  void Tick();

private:
  /**
   * @brief Initializes Targets and Sensors at specified positions.
   * @param target_positions The positions of targets in the simulation.
   * @param sensor_positions The positions of sensors in the simulation.
   */
  void PlaceAtPositions(const std::vector<Point> &target_positions, const std::vector<Point> &sensor_positions);
  /**
   * @brief Sorts indexes of targets and sensors by their positions.
   * @param target_idx A vector to hold the indices of targets.
   * @param sensor_idx A vector to hold the indices of sensors.
   */
  void SortByPositions(std::vector<size_t> &target_idx, std::vector<size_t> &sensor_idx);
  /**
   * @brief Determines the neighborhoods of sensors and targets.
   * @param targets_idx A vector holding the indices of targets.
   * @param sensors_idx A vector holding the indices of sensors.
   */
  void DetermineNeighborhoods(std::vector<size_t> &targets_idx, std::vector<size_t> &sensors_idx);
  /**
   * @brief Counts the coverage of targets by sensors and updates all_target_covered_ flag.
   * @return A vector of booleans indicating whether each target is covered.
   */
  std::vector<bool> CountCover();
};