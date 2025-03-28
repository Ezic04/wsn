#pragma once
#include <vector>
#include <cstdint>
#include <ranges>

class Id
{
  inline static uint32_t Counter;
  const uint32_t id_;

public:
  Id() : id_(Counter++) {}
  static uint32_t GetCounter();
  uint32_t GetId();
};

struct Point
{
  double x, y;
};

class Target : public Id, public Point
{

};

class Sensor: public Id
{
  inline static double Radius;
  Point position_;
  // std::vector<Point *> local_targets_;

public:
  Sensor(Point position);
  Sensor(const Sensor& other);
  Sensor& operator=(const Sensor& other);
  static void SetRadius(double radius);
  static double GetRadius();
  static uint32_t GetCounter();
  Point GetPosition() const;
};