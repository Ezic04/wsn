#pragma once
#include "core/Entity.hpp"
#include "core/utility.hpp"
#include "shared/utility.hpp"

/**
 * @brief Represents a target in the system.
 * @details The Target class inherits from Entity and Id<Target>.
 * It represents a target with a position and a cover flag indicating whether it is covered by a sensor.
 */
class Target : public Entity, public Id<Target>
{
  bool covered_flag_; ///< Flag indicating whether the target is covered by a sensor.

public:
  /**
   * @brief Constructs a Target with a given position.
   * @param position The initial position of the target.
   */
  explicit Target(Point position) : Entity(position), Id<Target>(), covered_flag_(false) {}
  Target(const Target &other) = default;                 ///< Copy constructor for Target.
  void SetCoverFlag(bool flag) { covered_flag_ = flag; } ///< Sets the cover flag of the target.
  bool GetCoverFlag() const { return covered_flag_; }    ///< Gets the cover flag of the target.
};
