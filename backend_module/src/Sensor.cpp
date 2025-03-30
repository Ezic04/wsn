#include "Sensor.hpp"

std::ostream& operator<<(std::ostream& os, const Point& p)
{
  return os << "(" << p.x << "," << p.y << ")";
}