#pragma once
#include <cstdint>
#include <vector>
#include "shared/utility.hpp"
/**
 * @file cover_structures.hpp
 * @brief Defines the Cover structure and related types for the sensor network.
 */

class Sensor;

using bit_vec = uint32_t;

constexpr static uint8_t bit_vec_size = 24;

/**
 * @class Cover
 * @brief Represents a cover in the sensor network.
 * @details A cover consists of a set of sensors, its degree,
 * lifetime (duration it can remain active), remaining sensors to turn on,
 * and the minimum sensor ID in the cover.
 */
struct Cover
{
  std::vector<Sensor *> sensors; ///< List of sensors in the cover
  uint16_t degree;               ///< Degree of the cover in LDGraph
  uint16_t lifetime;             ///< Lifetime of the cover in ticks
  uint16_t remaining_to_on;      ///< Number of sensors that need to be turned on
  uint32_t min_id;               ///< Minimum sensor ID in the cover
  bool feasible = true;          ///< Indicates if the cover is feasible

  /**
   * @brief Less-than operator for comparing two covers.
   *
   * This operator realizes a priority function for covers.
   *
   * @param other The other cover to compare against.
   * @return true if this cover has higher priority than the other cover,
   *         false otherwise.
   */
  bool operator<(const Cover &other) const
  {
    if (feasible != other.feasible) return feasible > other.feasible;
    if (degree != other.degree) return degree < other.degree;
    if (lifetime != other.lifetime) return lifetime > other.lifetime;
    if (remaining_to_on != other.remaining_to_on) return remaining_to_on < other.remaining_to_on;
    return min_id < other.min_id;
  }
  /**
   * @brief Checks if a sensor is part of this cover.
   *
   * @param sensor Pointer to the sensor to check.
   * @return true if the sensor is in the cover, false otherwise.
   */
  bool Contains(Sensor *sensor) const
  {
    return std::ranges::find(sensors, sensor) != sensors.end();
  }
};

using Edge = std::pair<uint32_t, uint16_t>; ///< Pair in LDGraph of the form (cover_idx, weight)
using Adjacent = std::vector<Edge>;         ///< List of edges (vertex ~ edges)
using LDGraph = std::vector<Adjacent>;      ///< List of vertices in LDGraph, each vertex contains a list of edges
                                            ///< This representation relies on covers stored in std::vector<Cover>