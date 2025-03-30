#pragma once
#include <vector>
#include <cstdint>
#include <ranges>
#include <ostream>
#include <utils.hpp>

class Entity
{
protected:
  Point position_;

public:
  explicit Entity(Point position) : position_(position) {}
  Point GetPosition() const { return position_; }
};

class Target : public Entity, public Id<Target>
{
public:
  explicit Target(Point position) : Entity(position), Id<Target>() {}
  Target(const Target &other) = default;
};

class Sensor : public Entity, public Id<Sensor>
{
  inline static double Radius;
  uint16_t battery_lvl_;
  std::vector<Target *> local_targets_;
  
public:
  std::vector<Sensor *> local_sensors_;
  explicit Sensor(Point position) : Entity(position), Id<Sensor>() {}
  Sensor(const Sensor &other) = default;
  static void SetRadius(double radius) { Radius = radius; }
  static double GetRadius() { return Radius; }
  void AddLocalTarget(Target &target) { local_targets_.push_back(&target); }
  void AddLocalSensor(Sensor &sensor) { local_sensors_.push_back(&sensor); }
};