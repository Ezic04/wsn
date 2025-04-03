#pragma once
#include <ostream>

template <typename T>
class Id
{
  inline static uint32_t Counter = 0;
  uint32_t id_;

public:
  Id() : id_(Counter++) {}
  static uint32_t GetCounter() { return Counter; }
  uint32_t GetId() const { return id_; }
};

struct Point
{
  double x, y; 
  Point operator*( double scale) const;
  Point& operator*=( double scale);
  friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

template<typename T>
T Sqr(T x)
{
  return x*x;
}