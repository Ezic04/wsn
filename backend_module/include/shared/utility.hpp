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

template <typename T>
class Id
{
public:
  using id_t = uint32_t;

private:
  inline static id_t Counter = 0;
  id_t id_;

protected:
  Id() : id_(Counter++) {}

public:
  static uint32_t GetIdCounter() { return Counter; }
  uint32_t GetId() const { return id_; }
  bool operator<(const Id<T> &other) { return id_ < other.GetId(); }
};

template <typename T>
T Sqr(T x)
{
  return x * x;
}