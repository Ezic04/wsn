#pragma once
#include "core/Sensor.hpp"
#include <vector>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <iostream> //for debug
#include "shared/utility.hpp"
#include "shared/data_structures.hpp"

class Simulation
{
public: // to remove
  uint32_t reshuffle_interval_ = 8;
  uint32_t initial_batterry_lvl_ = 64;
  uint32_t tick_;
  uint32_t covered_tragets_count_;
  bool all_target_covered_;
  std::vector<Target> targets_;
  std::vector<Sensor> sensors_;
  size_t target_num;
  size_t sensor_num;

public:
  Simulation() : tick_(-1), all_target_covered_(false), covered_tragets_count_(0) {}
  void Initialize(const SimulationParameters &parameters, const SimulationScenario &scenario);
  void Initialize(uint32_t target_num, uint32_t sensor_num, double sensor_radious);
  SimulationState GetSimulationState();
  void RunSimulation();
  void Tick();

private:
  void PlaceAtPositions(const std::vector<Point> &target_positions, const std::vector<Point> &sensor_positions);
  void PlaceSensors(uint32_t target_num, uint32_t sensor_num);
  void SortByPositions();
  void SortById();
  void DetermineNeighborhoods();
  std::vector<bool> CountCover();
};