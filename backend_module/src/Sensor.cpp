#include "Sensor.hpp"

uint32_t Id::GetCounter()
{
  return Counter;
}

uint32_t Id::GetId()
{
  return id_;
}

std::ostream& operator<<(std::ostream& os, const Point& p)
{
  return os << "(" << p.x << "," << p.y << ")";
}

Target::Target(Point position) : position_(position)
{
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

void Sensor::AddLocalTarget(Target *target)
{
  local_targets_.emplace_back(target);
}

void Sensor::AddLocalSensor(Sensor *sensor)
{
  local_sensors_.emplace_back(sensor);
}
