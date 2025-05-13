#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <utils.hpp>

class Sensor;

using bit_vec = uint16_t;

constexpr static uint8_t bit_vec_size = 16;

struct Cover
{
  std::vector<Sensor *> sensors;
  uint16_t degree;
  uint16_t lifetime;
  uint16_t remaining_on; // for this cover
  uint32_t min_id;
  bool feasible = true;
  
  bool operator<(const Cover& other) const 
  {
    if (degree != other.degree) return degree > other.degree;
    if (lifetime != other.lifetime) return lifetime < other.lifetime;
    if (remaining_on != other.remaining_on) return remaining_on > other.remaining_on;
    return min_id > other.min_id;
  }
};

// using CoverData = std::vector<Cover>; // data about covers with given Id 

using Edge = std::pair<uint32_t, uint16_t>; // (cover_idx, weight)

using Adjacent = std::vector<Edge>; // list of adj vertex (vertex ~ edges)

using LDGraph =  std::vector<Adjacent>; // index = cover id
