#pragma once
#include <limits>
#include <vector>
#include <unordered_map>

#include "core/cover_structures.hpp"
#include "core/Sensor.hpp"

class LDGraphGenerator
{
  std::vector<Sensor *> sensors_;
  std::vector<Target *> targets_;
  size_t sensor_num_;
  size_t target_num_;
  std::vector<bit_vec> sensor_cover_masks_;
  std::vector<bit_vec> cover_masks_;
  std::vector<Cover> covers_;
  LDGraph graph_;

public:
  LDGraphGenerator(std::vector<Sensor *> &sensors, std::vector<Target *> &targets);
  std::pair<std::vector<Cover>, LDGraph> operator()();

private:
  std::vector<Sensor *> MaskToSensors(bit_vec mask);
  void GenerateSensorCoverMasks();
  void GenerateMinimalCoverMasks();
  void InitializeCoverData();
  void GenerateLDGraph();
  void GenetateCoverData();
  void f();
};
