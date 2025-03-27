#include "Sensor.hpp"

Sensor::Sensor(Point position) : position_(position)
{
}

void Sensor::SetRadius(float radius)
{
  static bool initialized = false;
  if (!initialized)
  {
    Radius = radius;
    initialized = true;
  }
}

int add(int a, int b)
{
    return a + b;
}