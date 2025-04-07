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

public:
  Simulation(){};
  void Initialization(uint16_t target_num, uint16_t sensor_num, double sensor_radious);
  void RunSimulation();
private:
  void SelectRandomPositions(uint16_t target_num, uint16_t sensor_num);
  void SortByPositions();
  void SelectNeighborhoods();
};