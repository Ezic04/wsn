#include "core/GenerateLDGraph.hpp"

LDGraphGenerator::LDGraphGenerator(std::vector<Sensor *> &sensors, std::vector<Target *> &targets) : sensors_(sensors), targets_(targets), sensor_num_(sensors.size()), target_num_(targets.size()),
                                                                                                     sensor_cover_masks_(), cover_masks_(), covers_(), graph_()
{
}

std::pair<std::vector<Cover>, LDGraph> LDGraphGenerator::operator()()
{
  GenerateSensorCoverMasks();
  GenerateMinimalCoverMasks();
  InitializeCoverData();
  GenerateLDGraph();
  GenetateCoverData();
  return std::pair(covers_, graph_);
}

std::vector<Sensor *> LDGraphGenerator::MaskToSensors(bit_vec mask)
{
  std::vector<Sensor *> result;
  bit_vec rem = mask;
  while (rem)
  {
    int i = std::countr_zero(rem);
    rem &= (rem - 1);
    result.push_back(sensors_[i]);
  }
  return result;
}

void LDGraphGenerator::GenerateSensorCoverMasks()
{
  int i = 0;
  for (auto *sensor : sensors_)
  {
    bit_vec mask = 0;
    for (auto *target : targets_)
    {
      mask <<= 1;
      mask |= sensor->IsLocalTarget(*target);
    }
    sensor_cover_masks_.emplace_back(mask);
  }
}

void LDGraphGenerator::GenerateMinimalCoverMasks()
{
  bit_vec full_cover = (1ULL << target_num_) - 1;
  bit_vec full_sensor = (1ULL << sensor_num_) - 1;

  std::unordered_map<bit_vec, bool> lookup_table;

  auto is_cover = [&](bit_vec candidate) -> bool
  {
    bit_vec cover_mask = 0;
    bit_vec rem = candidate;
    while (rem)
    {
      int i = std::countr_zero(rem);
      rem &= (rem - 1);
      cover_mask |= sensor_cover_masks_[i];
    }
    return (~cover_mask & full_cover) == 0;
  };

  auto minimal_covers_aux = [&](auto self, bit_vec candidate) -> bool
  {
    if (lookup_table.contains(candidate))
    {
      return lookup_table[candidate];
    }
    if (!is_cover(candidate))
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
      return lookup_table[candidate] = true;
    }
    cover_masks_.emplace_back(candidate);
    return lookup_table[candidate] = true;
  };
  minimal_covers_aux(minimal_covers_aux, full_sensor);
}

void LDGraphGenerator::InitializeCoverData()
{
  for (bit_vec mask : cover_masks_)
  {
    auto sensors_in_cover = MaskToSensors(mask);
    covers_.emplace_back(sensors_in_cover, 0, 0, 0, 0);
  }
}

void LDGraphGenerator::GenerateLDGraph()
{
  graph_ = LDGraph(covers_.size(), Adjacent());

  for (size_t i = 0; i < covers_.size(); ++i)
  {
    for (size_t j = i + 1; j < covers_.size(); ++j)
    {
      bit_vec intersection_mask = cover_masks_[i] & cover_masks_[j];
      if (intersection_mask == 0)
      {
        continue;
      }
      uint16_t weight = std::numeric_limits<uint16_t>::max();
      const std::vector<Sensor *> intersection = MaskToSensors(intersection_mask);
      for (const Sensor *sensor : intersection)
      {
        weight = std::min(weight, sensor->GetBateryLevel());
      }
      graph_[i].emplace_back(j, weight);
      graph_[j].emplace_back(i, weight);
    }
  }
}

void LDGraphGenerator::GenetateCoverData()
{
  for (size_t i = 0; i < covers_.size(); ++i)
  {
    Adjacent &adj = graph_[i];
    Cover &cover = covers_[i];
    cover.degree = 0;
    for (const auto &[_, weight] : adj)
    {
      cover.degree += weight; // could be done once
    }
    cover.lifetime = std::numeric_limits<uint16_t>::max();
    cover.remaining_to_on = cover.sensors.size();
    cover.min_id = std::numeric_limits<uint32_t>::max();
    for (const Sensor *sensor : cover.sensors)
    {
      cover.lifetime = std::min(cover.lifetime, sensor->GetBateryLevel());
      cover.min_id = std::min(cover.min_id, sensor->GetId()); // could be done once
      cover.feasible = false;
    }
  }
  std::sort(covers_.begin(), covers_.end());
}