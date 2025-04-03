#include "utils.hpp"

Point Point::operator*(double scale) const
{
  return Point(scale * x, scale * y);
}

Point &Point::operator*=(double scale)
{
  x *= scale;
  y *= scale;
  return *this;
}

std::ostream &operator<<(std::ostream &os, const Point &p)
{
  return os << "(" << p.x << ", " << p.y << ")";
}
