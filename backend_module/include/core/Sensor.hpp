#pragma once
#include <vector>
#include <unordered_set>
#include <optional>
#include <ranges>
#include <algorithm>
#include <stdexcept>
#include <format>

#include "shared/utility.hpp"
#include "core/Entity.hpp"
#include "core/Target.hpp"
#include "core/utility.hpp"
#include "core/cover_structures.hpp"
/**
 * @file Sensor.hpp
 * @brief Defines the Sensor class, which represents a sensor in the system.
 */

/**
 * @class Sensor
 * @brief Represents a sensor in the system.
 * @details The Sensor class inherits from Entity and Id<Sensor>.
 * It represents a sensor with a position, battery level, state,
 * and local targets and sensors. It also manages covers and their reshuffling.
 */
class Sensor : public Entity, public Id<Sensor>
{
public:
  enum class State ///< The current state of the sensor.
  {
    kOn,
    kOff,
    kDead,
    kUndecided,
  };

private:
  inline static double Radius;          ///< The sensing radius of the sensor
  uint16_t battery_lvl_;                ///< The battery level of the sensor
  State state_;                         ///< The current state of the sensor
  std::vector<Target *> local_targets_; ///< List of local targets that the sensor can detect
  std::vector<Sensor *> local_sensors_; ///< List of local sensors that the sensor can communicate with
  LDGraph local_graph_;                 ///< LDGraph representing possible coverages and relationships between them
  std::vector<Cover> covers_;           ///< List of minimal covers visible to the sensor
  Cover *current_cover_;                ///< Pointer to the current best cover
  size_t current_cover_idx_;            ///< Index of the current cover in the covers list

public:
  /**
   * @brief Constructs a Sensor with a given position and battery level.
   * @param position The initial position of the sensor.
   * @param battery_lvl The initial battery level of the sensor.
   */
  Sensor(Point position, uint32_t battery_lvl) : Entity(position), Id<Sensor>(), battery_lvl_(battery_lvl), state_(State::kUndecided) {}
  Sensor(const Sensor &other) = default; ///< Copy constructor for Sensor.
  /**
   * @brief Initializes the sensor.
   * @details This method creates a local graph for the sensor and initializes its covers.
   * @exception Throws std::runtime_error if number of targets or sensors is greater than bit_vec_size.
   */
  void Initialize();
  inline static void SetRadius(double radius) { Radius = radius; }           ///< Sets the sensing radius of the sensor.
  inline static double GetRadius() { return Radius; }                        ///< Gets the sensing radius of the sensor.
  inline State GetState() const { return state_; }                           ///< Gets the current state of the sensor.
  inline uint16_t GetBatteryLevel() const { return battery_lvl_; }           ///< Gets the battery level of the sensor.
  inline std::vector<Target *> &GetLocalTargets() { return local_targets_; } ///< Gets the list of local targets that the sensor can detect.
  /**
   * @brief Checks if a target is a local target.
   * @param target The target to check.
   * @return True if the target is local, false otherwise.
   */
  inline bool IsLocalTarget(Target &target) const { return std::ranges::find(local_targets_, &target) != local_targets_.end(); }
  inline void SetState(State state) { state_ = state; } ///< Sets the current state of the sensor.
  /**
   * @brief Adds a local target to the sensor.
   * @param target The target to add.
   */
  inline void AddLocalTarget(Target &target) { local_targets_.emplace_back(&target); }
  /**
   * @brief Adds a local sensor to the sensor.
   * @param sensor The sensor to add.
   */
  inline void AddLocalSensor(Sensor &sensor) { local_sensors_.emplace_back(&sensor); }
  /**
   * @brief Updates the sensor's state.
   * @note this should be called every tick of the simulation.
   */
  void Update();
  /**
   * @brief Begins the reshuffle process for the sensor.
   * @note This should be called before calling Reshuffle().
   */
  void BeginReshuffle();
  /**
   * @brief Chooses the best cover for the sensor and sets the sensor's state accordingly.
   * @details This method selects the best cover based on the current local graph and updates the sensor's state.
   */
  bool Reshuffle();

private:
  /**
   * @brief Updates the local graph of the sensor.
   * @details This method updates the local graph based on the current local targets and sensors.
   */
  void UpdateCoverData();
};