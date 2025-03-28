#include "Sensor.hpp"

uint32_t Id::GetCounter()
{
  return Counter;
}

uint32_t Id::GetId()
{
  return id_;
}

Sensor::Sensor(Point position) : position_(position)
{
}

Sensor::Sensor(const Sensor &other)
{
  *this = other;
}

Sensor &Sensor::operator=(const Sensor &other)
{
  if (this != &other)
  {
    position_ = other.position_;
  }
  return *this;
}

void Sensor::SetRadius(double radius)
{
  static bool initialized = false;
  if (!initialized)
  {
    Radius = radius;
    initialized = true;
  }
}

double Sensor::GetRadius()
{
  return Radius;
}

Point Sensor::GetPosition() const
{
  return position_;
}
