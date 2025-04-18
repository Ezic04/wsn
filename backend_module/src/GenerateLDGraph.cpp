#include "GenerateLDGraph.hpp"

GenerateLDGraph::GenerateLDGraph(std::set<Sensor *>& sensors, std::set<Target *>& targets):
sensors_(sensors), targets_(targets), sensor_num_(sensors.size()), target_num_(targets.size())
{
}

LDGraph GenerateLDGraph::operator()()
{
  GenerateSensorCoverMasks();
  GenerateMinimalCoverages();

  // for (int i = 0; i < covers.size(); ++i)
  // {
  //   std::vector<Edge> incident;
  //   for (int j = 0; j < covers.size(); ++j)
  //   {
  //     if (i != j)
  //     {
  //       uint32_t weight = UINT32_MAX;
  //       std::vector<Sensor *> intersection;
  //       for (const auto &x : covers[i].sensors_)
  //       {
  //         if (covers[j].sensors_.contains(x)) // to change
  //         {
  //           intersection.emplace_back(x);
  //         }
  //       }
  //       if (intersection.empty())
  //       {
  //         continue;
  //       }
  //       for (const auto &it : intersection)
  //       {
  //         weight = std::min(weight, it->GetBateryLevel());
  //       }
  //       incident.emplace_back(&covers[j], weight);
  //     }
  //   }
  //   local_graph_[covers[i]] = incident;
  // }
  return LDGraph();
}


void GenerateLDGraph::GenerateSensorCoverMasks()
{
  int i = 0;
  for (auto* s : sensors_)
  {
    bit_vec mask = 0;
    for (auto* t : targets_)
    {
      mask <<= 1;
      mask |= s->IsLocalTarget(*t);
    }
    sensor_cover_masks_[i++] = mask;
  }
}

void GenerateLDGraph::GenerateMinimalCoverages()
{
  bit_vec full_cover = (1ULL << target_num_) - 1;
  bit_vec full_sensor = (1ULL << sensor_num_) - 1;

  std::unordered_map<bit_vec, bool> lookup_table;

  auto is_cover = [&](bit_vec candidate) -> bool
  {
    bit_vec cover_mask = 0;
    for (size_t i = 0; i < sensor_num_; ++i)
    {
      if (candidate & (1ULL << i))
      {
        cover_mask |= sensor_cover_masks_[i];
      }
    }
    return (~cover_mask & full_cover) == 0;
  };

  auto minimal_covers_aux = [&](auto self, bit_vec candidate) -> bool
  {
    if (lookup_table.contains(candidate))
    {
      return lookup_table[candidate];
    }
    if (candidate == 0 || !is_cover(candidate))
    {
      return lookup_table[candidate] = false;
    }
    bool is_minimal = true;
    bit_vec rem = candidate;
    while (rem)
    {
      int i = std::countr_zero(rem);
      rem &= (rem - 1);
      bit_vec new_candidate = candidate & ~(1ULL << i);
      if (self(self, new_candidate))
      {
        is_minimal = false;
      }
    }
    if (!is_minimal)
    {
      return lookup_table[candidate] = false;
    }
    covers.emplace_back(candidate);
    return lookup_table[candidate] = true;
  };
  minimal_covers_aux(minimal_covers_aux, full_sensor);
}

