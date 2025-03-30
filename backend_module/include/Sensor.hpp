#pragma once
#include <vector>
#include <cstdint>
#include <ranges>
#include <ostream>

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
  friend std::ostream &operator<<(std::ostream &os, const Point &p);
};

class Target : public Id
{
  Point position_;

public:
  Target(Point position);
};

class Sensor : public Id
{
  inline static double Radius;
  Point position_;
  uint16_t battery_lvl_;
  std::vector<Target*> local_targets_;
  
  public:
  std::vector<Sensor*> local_sensors_;
  Sensor(Point position);
  Sensor(const Sensor &other);
  Sensor &operator=(const Sensor &other);
  static void SetRadius(double radius);
  static double GetRadius();
  Point GetPosition() const;
  void AddLocalTarget(Target* target);
  void AddLocalSensor(Sensor* sensor);
};