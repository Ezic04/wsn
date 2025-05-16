#pragma once
#include <Sensor.hpp>
#include <vector>
#include <stdexcept>
#include <random>
#include <algorithm>
#include <iostream> //for debug
#include <utils.hpp>

class Simulation
{
public: // to remove
  std::vector<Target> targets_;
  std::vector<Sensor> sensors_;
  uint32_t covered_tragets_count_;
  bool all_target_covered_ = true;
  uint32_t tick_ = 0;
  const uint32_t max_baterry_ = 64;
  const uint32_t reshuffle_interval_ = 8;

public:
  Simulation(){};
  void Initialization(uint16_t target_num, uint16_t sensor_num, double sensor_radious);
  void RunSimulation();
private:
  void PlaceSensors(uint16_t target_num, uint16_t sensor_num);
  void SortByPositions();
  void DetermineNeighborhoods();
  void Update();
  void CountCover();
};