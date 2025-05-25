#pragma once
#include "core/Sensor.hpp"
#include <vector>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <iostream> //for debug
#include "shared/utility.hpp"
#include "shared/simulation_structures.hpp"

class Simulation
{
  uint32_t reshuffle_interval_;
  uint32_t initial_batterry_lvl_;
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
  // void Initialize(uint32_t target_num, uint32_t sensor_num, double sensor_radious);
  SimulationState GetSimulationState();
  void RunSimulation();
  void Tick();

private:
  void PlaceAtPositions(const std::vector<Point> &target_positions, const std::vector<Point> &sensor_positions);
  // void PlaceSensors(uint32_t target_num, uint32_t sensor_num);
  void SortByPositions(std::vector<size_t> &target_idx,std::vector<size_t> &sensord_idx);
  void SortById();
  void DetermineNeighborhoods(std::vector<size_t> &targets_idx, std::vector<size_t> &sensors_idx);
  std::vector<bool> CountCover();
};