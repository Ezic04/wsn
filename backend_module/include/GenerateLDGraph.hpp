#pragma once
#include <vector>
#include <array>
#include <set>
#include <map>
#include <unordered_map>

#include <LDGraph.hpp>
#include <Sensor.hpp>

using bit_vec = uint16_t;
constexpr static uint8_t max_bit_vec = 16;

class GenerateLDGraph
{
  std::set<Sensor *>& sensors_;
  std::set<Target *>& targets_;
  size_t target_num_;
  size_t sensor_num_;
  std::array<bit_vec, max_bit_vec> sensor_cover_masks_;
  std::vector<bit_vec> covers;

public:
  GenerateLDGraph(std::set<Sensor *>& sensors, std::set<Target *>& targets);
  LDGraph operator()();

private:
  void GenerateSensorCoverMasks();
  void GenerateMinimalCoverages();
};