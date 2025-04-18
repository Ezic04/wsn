#pragma once
#include <cstdint>
#include <map>
#include <set>

class Sensor;

struct Cover
{
  std::set<Sensor *> sensors_;
  uint32_t lt;
  bool operator<(const Cover &other) const { return lt < other.lt; }
};

using Edge = std::pair<Cover *, uint32_t>;
using LDGraph = std::map<Cover, std::vector<Edge>>;