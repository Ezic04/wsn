#pragma once
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <ostream>
#include <format>
#include <ranges>
#include <algorithm>
#include <optional>

#include "shared/utility.hpp"
#include "core/Entity.hpp"
#include "core/Target.hpp"
#include "core/utility.hpp"
#include "core/cover_structures.hpp"

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
  Cover* current_cover_;
  size_t current_cover_idx;

public:
  Sensor(Point position, uint32_t battery_lvl) : Entity(position), Id<Sensor>(), battery_lvl_(battery_lvl), state_(State::kUndecided) {}
  Sensor(const Sensor &other) = default;
  void Initialize();
  inline static void SetRadius(double radius) { Radius = radius; }
  inline static double GetRadius() { return Radius; }
  inline State GetState() const { return state_; }
  inline uint16_t GetBateryLevel() const { return battery_lvl_; }
  inline std::vector<Target *>& GetLocalTargets() { return local_targets_; }
  inline bool IsLocalTarget(Target &target) const { return std::ranges::find(local_targets_, &target) != local_targets_.end(); }
  inline void SetState(State state) { state_ = state; }
  inline void AddLocalTarget(Target &target) { local_targets_.emplace_back(&target); }
  inline void AddLocalSensor(Sensor &sensor) { local_sensors_.emplace_back(&sensor); }
  void Update();
  void BeginReshuffle();
  bool Reshuffle();
private:
  void UpdateCoverData();
};