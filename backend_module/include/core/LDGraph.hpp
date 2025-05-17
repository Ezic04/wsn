#pragma once
#include <cstdint>
#include <vector>
#include "shared/utility.hpp"

class Sensor;

using bit_vec = uint32_t;

constexpr static uint8_t bit_vec_size = 32;

struct Cover
{
  std::vector<Sensor *> sensors;
  uint16_t degree;
  uint16_t lifetime;
  uint16_t remaining_to_on; // for this cover
  uint32_t min_id;
  bool feasible = true;
  
  bool operator<(const Cover& other) const 
  {
    if (degree != other.degree) return degree < other.degree;
    if (lifetime != other.lifetime) return lifetime > other.lifetime;
    if (remaining_to_on != other.remaining_to_on) return remaining_to_on < other.remaining_to_on;
    return min_id < other.min_id;
  }
  bool Contains(Sensor* sensor) const
  {
    return std::ranges::find(sensors, sensor) != sensors.end();
  }

  // bool IsSatisfied(Sensor* self) const {
  // for (Sensor* s : sensors) {
  //   if (s == self && !contains(self)) // nie należy do pokrycia
  //     continue;
  //   if (s->GetState() != State::kOn)
  //     return false;
  // }
  // return true;
// }
};

// using CoverData = std::vector<Cover>; // data about covers with given Id 

using Edge = std::pair<uint32_t, uint16_t>; // (cover_idx, weight)

using Adjacent = std::vector<Edge>; // list of adj vertex (vertex ~ edges)

using LDGraph =  std::vector<Adjacent>; // index = cover id
