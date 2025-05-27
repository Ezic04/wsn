#pragma once
#include "shared/utility.hpp"
/**
 * @file Entity.hpp
 * @brief Defines the Entity class, which represents an entity in the system.
 */

/**
 * @class Entity
 * @brief Represents an entity in the system with a position.
 * @note This class is intended to be a base class for other entities in the system.
 */
class Entity
{
protected:
  Point position_; ///< Position of the entity in 2D space.

public:
  /**
   * @brief Constructs an Entity with a given position.
   * @param position The initial position of the entity.
   */
  explicit Entity(Point position) : position_(position) {}
  /**
   * @brief Gets the position of the entity.
   * @return The current position of the entity.
   */
  Point GetPosition() const { return position_; }
};
