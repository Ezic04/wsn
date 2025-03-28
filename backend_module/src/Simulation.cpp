#include <Simulation.hpp>

void Simulation::Initialization(uint16_t target_num, uint16_t sensor_num, float sensor_radious)
{
  if (!(0.0 < sensor_radious && sensor_radious < 1.0))
  {
    throw std::invalid_argument("sensor_radious must be a small number between 0 and 1");
  }
  SelectPositions(target_num, sensor_num);
  Sensor::SetRadius(0.005);
}

void Simulation::SelectPositions(uint16_t target_num, uint16_t sensor_num)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(0.0, 1.0);
  targets_.reserve(target_num);
  for (size_t i = 0; i < target_num; i++)
  {
    targets_.emplace_back(dist(gen), dist(gen));
  }
  sensors_.reserve(sensor_num);
  for (size_t i = 0; i < sensor_num; i++)
  {
    targets_.emplace_back(dist(gen), dist(gen));
  }
}

void Simulation::SelectNeighborhoods()
{
  auto PointCompare = [](const Point& p1, const Point& p2) -> bool
  {
    return std::fabs(p1.x - p2.x) < Sensor::GetRadius() ? p1.y < p2.y : p1.x < p2.x;
  };
  auto SensorCompare = [&PointCompare](const Sensor& s1, const Sensor& s2) -> bool
  {
    return PointCompare(s1.GetPosition(), s2.GetPosition());
  };
  std::sort(targets_.begin(), targets_.end(), PointCompare);
  std::sort(sensors_.begin(), sensors_.end(), SensorCompare);
  
}
