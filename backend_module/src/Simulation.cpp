#include <Simulation.hpp>

Simulation::Simulation() : targets_(), sensors_()
{
}

void Simulation::Initialization(uint16_t target_num, uint16_t sensor_num, double sensor_radious)
{
  if (!(0.0 < sensor_radious && sensor_radious < 1.0))
  {
    throw std::invalid_argument("sensor_radious must be a small number between 0 and 1");
  }
  SelectPositions(target_num, sensor_num);
  Sensor::SetRadius(sensor_radious);
  SelectNeighborhoods();
  // for (const auto &it : targets_)
  // {
  //   std::cout << it.GetPosition() << '\n';
  // }
  // std::cout << "-----------------\n";
  for (const auto &it : sensors_)
  {
    std::cout << it.GetId() << " " << it.GetPosition() << ": ";
    for (size_t j = 0; j < it.local_sensors_.size(); j++)
    {
      std::cout << it.local_sensors_[j]->GetId() << ", ";
    }
    std::cout << '\n';
  }
}

void Simulation::SelectPositions(uint16_t target_num, uint16_t sensor_num)
{
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> dist(0.0, 1.0);
  targets_.reserve(target_num);
  for (size_t i = 0; i < target_num; i++)
  {
    targets_.emplace_back(Point(dist(gen), dist(gen)));
  }
  sensors_.reserve(sensor_num);
  for (size_t i = 0; i < sensor_num; i++)
  {
    sensors_.emplace_back(Point(dist(gen), dist(gen)));
  }
}

void Simulation::SelectNeighborhoods()
{
  auto PointCompare = [](const Point &p1, const Point &p2) -> bool
  {
    return std::fabs(p1.x - p2.x) < Sensor::GetRadius() ? p1.y < p2.y : p1.x < p2.x;
  };
  auto TargetCompare = [&PointCompare](const Target &t1, const Target &t2) -> bool
  {
    return PointCompare(t1.GetPosition(), t2.GetPosition());
  };  
  auto SensorCompare = [&PointCompare](const Sensor &s1, const Sensor &s2) -> bool
  {
    return PointCompare(s1.GetPosition(), s2.GetPosition());
  };
  std::sort(targets_.begin(), targets_.end(), TargetCompare);
  std::sort(sensors_.begin(), sensors_.end(), SensorCompare); // should be in other method
  for (auto s = sensors_.begin(); s != sensors_.end(); ++s)
  {
    auto it = s+1;
    if (it == sensors_.end())
    {
      break;
    }
    Point s_pos = s->GetPosition();
    Point it_pos = it->GetPosition();
    while ( it != sensors_.end() && it_pos.x < s_pos.x + Sensor::GetRadius())
    {
      it_pos =it->GetPosition();
      if (Sqr(s_pos.x - it_pos.x) + Sqr(s_pos.y - it_pos.y) < Sqr(Sensor::GetRadius()))
      {
        s->AddLocalSensor(*it);
        it->AddLocalSensor(*s);
      }
      ++it;
    }
  }
}
