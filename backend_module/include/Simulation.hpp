#pragma once
#include <Sensor.hpp>
#include <vector>
#include <stdexcept>
#include <random>
#include <algorithm>


class Simulation
{
  std::vector<Point> targets_;
  std::vector<Sensor> sensors_;
  void Initialization(uint16_t target_num, uint16_t sensor_num, float sensor_radious);
  void SelectPositions(uint16_t target_num, uint16_t sensor_num);
  void SelectNeighborhoods();
};