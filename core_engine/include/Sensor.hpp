#pragma once

struct Point
{
  float x, y;
};

class Sensor
{
  Point position_;
public:
  inline static float Radius;
  Sensor(Point position);
  static void SetRadius(float radius);
};