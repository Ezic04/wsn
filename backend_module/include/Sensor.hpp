#pragma once
#include <cstdint>
#include <ostream>
#include <ranges>
#include <utils.hpp>
#include <vector>

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
public:
  enum class State
  {
    kInactive,
    kActive,
    kDead
  };

private:
  inline static double Radius;
  int16_t battery_lvl_;
  State state_;
  std::vector<Target *> local_targets_;
  std::vector<Sensor *> local_sensors_;

public:
  Sensor(Point position, int16_t battery_lvl) : Entity(position), Id<Sensor>(), battery_lvl_(battery_lvl), state_(State::kActive) {}
  Sensor(const Sensor &other) = default;
  static void SetRadius(double radius) { Radius = radius; }
  static double GetRadius() { return Radius; }
  State GetState() const { return state_; }
  void AddLocalTarget(Target &target) { local_targets_.push_back(&target); }
  void AddLocalSensor(Sensor &sensor) { local_sensors_.push_back(&sensor); }
  void Execute();
};