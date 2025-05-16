#pragma once
#include <ostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <format>
#include <ranges>
#include <algorithm>

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
  bool covered_flag_;

public:
  explicit Target(Point position) : Entity(position), Id<Target>(), covered_flag_(false) {}
  Target(const Target &other) = default;
  void SetCoverFlag(bool flag) { covered_flag_ = flag; }
  bool GetCoverFlag() const { return covered_flag_; }
};

class Sensor : public Entity, public Id<Sensor>
{
public:
  enum class State
  {
    kOn,
    kOff,
    kDead,
    kUndecided,
  };

private:
  inline static double Radius;
  uint16_t battery_lvl_;
  State state_;

  std::vector<Target *> local_targets_;
  std::vector<Sensor *> local_sensors_;
  LDGraph local_graph_;
  std::vector<Cover> covers_;
  size_t current_cover_index_;

public:
  Sensor(Point position, uint32_t battery_lvl) : Entity(position), Id<Sensor>(), battery_lvl_(battery_lvl), state_(State::kUndecided) {}
  Sensor(const Sensor &other) = default;
  void Initialization();
  static void SetRadius(double radius) { Radius = radius; }
  static double GetRadius() { return Radius; }
  State GetState() const { return state_; }
  uint16_t GetBateryLevel() const { return battery_lvl_; }
  bool IsLocalTarget(Target &target) const { return std::ranges::find(local_targets_, &target) != local_targets_.end(); }
  void AddLocalTarget(Target &target) { local_targets_.emplace_back(&target); }
  void AddLocalSensor(Sensor &sensor) { local_sensors_.emplace_back(&sensor); }
  void Update();
  void BeginReshuffle();
  bool Reshuffle();
private:
  void UpdateCoverData();
};