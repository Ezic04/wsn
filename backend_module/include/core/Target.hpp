#pragma once
#include "core/Entity.hpp"
#include "core/utility.hpp"
#include "shared/utility.hpp"

class Target : public Entity, public Id<Target>
{
  bool covered_flag_;

public:
  explicit Target(Point position) : Entity(position), Id<Target>(), covered_flag_(false) {}
  Target(const Target &other) = default;
  void SetCoverFlag(bool flag) { covered_flag_ = flag; }
  bool GetCoverFlag() const { return covered_flag_; }
};
