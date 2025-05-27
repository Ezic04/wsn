#pragma once
#include <ostream>

/**
 * @brief Represents a 2D point with x and y coordinates.
 *
 * This struct provides basic operations for scaling a point and
 * outputting its representation to an output stream.
 */
struct Point
{
  double x, y; ///< Coordinates of the point in 2D space.

  /**
   * @brief Constructs a Point with given x and y coordinates.
   *
   * @param x The x coordinate of the point.
   * @param y The y coordinate of the point.
   */
  Point operator*(double scale) const
  {
    return Point(scale * x, scale * y);
  }
  /**
   * @brief Scales the point by a given factor.
   *
   * @param scale The scaling factor.
   * @return A reference to the scaled point.
   */
  Point &operator*=(double scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }
  /**
   * @brief Outputs the point to an output stream.
   *
   * @param os The output stream to write to.
   * @param p The point to output.
   * @return The output stream after writing the point.
   */
  friend std::ostream &operator<<(std::ostream &os, const Point &p)
  {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};
