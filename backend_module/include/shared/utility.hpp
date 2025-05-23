#pragma once
#include <ostream>

struct Point
{
  double x, y;
  Point operator*(double scale) const
  {
    return Point(scale * x, scale * y);
  }
  Point &operator*=(double scale)
  {
    x *= scale;
    y *= scale;
    return *this;
  }
  friend std::ostream &operator<<(std::ostream &os, const Point &p)
  {
    return os << "(" << p.x << ", " << p.y << ")";
  }
};
