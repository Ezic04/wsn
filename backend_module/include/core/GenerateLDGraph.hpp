#pragma once
#include <limits>
#include <vector>
#include <unordered_map>

#include "core/cover_structures.hpp"
#include "core/Sensor.hpp"
/**
 * @file GenerateLDGraph.hpp
 * @brief Defines the LDGraphGenerator class, which generates a Localized Distributed Graph (LDGraph) for a set of sensors and targets.
 */

/**
 * @class LDGraphGenerator
 * @brief Generates a Localized Distributed Graph (LDGraph) for a set of sensors and targets.
 * @details The LDGraphGenerator class takes a set of sensors and targets, generates sensor cover masks, minimal cover masks,
 * and constructs the LDGraph based on the relationships between sensors and targets.
 */
class LDGraphGenerator
{
  std::vector<Sensor *> sensors_;           ///< List of sensors considered.
  std::vector<Target *> targets_;           ///< List of targets considered.
  size_t sensor_num_;                       ///< Number of sensors.
  size_t target_num_;                       ///< Number of targets.
  std::vector<bit_vec> sensor_cover_masks_; ///< Masks representing which sensors cover which targets.
  std::vector<bit_vec> cover_masks_;        ///< Masks representing minimal covers.
  std::vector<Cover> covers_;               ///< List of covers generated.
  LDGraph graph_;                           ///< Graph representing the relationships between sensors and covers.
  bit_vec full_cover;                       ///< Bitmask representing a full cover (all targets covered).
  bit_vec full_sensor;                      ///< Bitmask representing a full set of sensors (all sensors considered).

public:
  /**
   * @brief Constructs an LDGraphGenerator with a set of sensors and targets.
   * @param sensors A vector of pointers to Sensor objects.
   * @param targets A vector of pointers to Target objects.
   */
  LDGraphGenerator(std::vector<Sensor *> &sensors, std::vector<Target *> &targets);
  std::pair<std::vector<Cover>, LDGraph> operator()(); ///< Generates the LDGraph and covers based on the provided sensors and targets.

private:
  /**
   * @brief Converts a bitmask to a vector of Sensor pointers.
   * @param mask The bitmask representing the sensors.
   * @return A vector of pointers to the corresponding Sensor objects.
   */
  std::vector<Sensor *> MaskToSensors(bit_vec mask);
  /**
   * @brief Generates masks for sensors covering targets.
   * Each mask represents which targets are covered by a specific sensor.
   */
  void GenerateSensorCoverMasks();
  /**
   * @brief Generates minimal cover masks.
   * Each mask represents a minimal set of sensors that can cover all targets.
   * Minimal cover is defined as a set of sensors such that removing any sensor from the set would result in at least one target not being covered.
   */
  void GenerateMinimalCoverMasks();
  /**
   * @brief Initializes cover data for the sensors.
   */
  void InitializeCoverData();
  /**
   * @brief Generates the Localized Distributed Graph (LDGraph).
   * The graph is constructed based on interactions of covers (as a sets of sensors) with each other.
   */
  void GenerateLDGraph();
  /**
   * @brief Generates cover data for the sensors.
   * @details This method sets attributes of covers. This is caused by the fact that some of them depends on LDGraph structure.
   */
  void GenerateCoverData();
};
