#pragma once
#include <ostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <format>

#include <LDGraph.hpp>
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
  bool check_flag_;

public:
  explicit Target(Point position) : Entity(position), Id<Target>(), check_flag_(false) {}
  Target(const Target &other) = default;
  void SetCheckFlag(bool flag) { check_flag_ = flag; }
  bool GetCheckFlag() const { return check_flag_; }
};

class Sensor : public Entity, public Id<Sensor>
{
public:
  enum class State
  {
    kActive,
    kInactive,
    kDischarged
  };

private:
  inline static double Radius;
  uint32_t battery_lvl_;
  State state_;

  std::set<Target *> local_targets_;
  std::set<Sensor *> local_sensors_;
  LDGraph local_graph_;

public:
  Sensor(Point position, uint32_t battery_lvl) : Entity(position), Id<Sensor>(), battery_lvl_(battery_lvl), state_(State::kActive) {}
  Sensor(const Sensor &other) = default;
  void Initialization();
  static void SetRadius(double radius) { Radius = radius; }
  static double GetRadius() { return Radius; }
  State GetState() const { return state_; }
  uint32_t GetBateryLevel() { return battery_lvl_; }
  bool IsLocalTarget(Target &target) const { return local_targets_.contains(&target); }
  void AddLocalTarget(Target &target) { local_targets_.emplace(&target); }
  void AddLocalSensor(Sensor &sensor) { local_sensors_.emplace(&sensor); }
  void Update();
};