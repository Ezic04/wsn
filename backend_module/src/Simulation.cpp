#include <Simulation.hpp>

// #define DEBUG
#define RD 0

constexpr int battery = 32;

void Simulation::Initialization(uint16_t target_num, uint16_t sensor_num, double sensor_radious)
{
  if (!(0.0 < sensor_radious && sensor_radious < 1.0))
  {
    throw std::invalid_argument("sensor_radious must be a small number between 0 and 1");
  }
  #ifdef DEBUG
  Sensor::SetRadius(sensor_radious);
  targets_.emplace_back(Point(0.4, 0.4));
  targets_.emplace_back(Point(0.6, 0.6));
  sensors_.emplace_back(Point(0.3, 0.3), battery);
  sensors_.emplace_back(Point(0.5, 0.5), battery);
  sensors_.emplace_back(Point(0.7, 0.7), battery);
  #else
  Sensor::SetRadius(sensor_radious);
  PlaceSensors(target_num, sensor_num);
  #endif
  SortByPositions();
  DetermineNeighborhoods();
}

void Simulation::RunSimulation()
{
  auto sens_num = sensors_.size();
  auto targtet_num = targets_.size();
  bool all_target_flag = true;
  uint16_t counter = 0;
  for (auto &sensor : sensors_)
  {
    sensor.Initialization();
  }
  while (all_target_flag)
  {
    for (auto &s : sensors_)
    {
      s.Update();
    }
    for (auto& t : targets_)
    {
      if(!t.GetCheckFlag())
      {
        all_target_flag = false;
      }
      t.SetCheckFlag(false);
    }
    counter += (int)all_target_flag;
  }
  std::cout << "Network lifetime: " << counter << '\n';
}

void Simulation::PlaceSensors(uint16_t target_num, uint16_t sensor_num)
{
  std::random_device rd;
  #ifdef RD
  std::mt19937 gen(RD);
  #else
  std::mt19937 gen(rd());
  #endif
  std::uniform_real_distribution<> dist(0.0, 1.0);
  sensors_.reserve(sensor_num);
  targets_.reserve(target_num);
  for (size_t i = 0; i < sensor_num; ++i)
  {
    sensors_.emplace_back(Point(dist(gen), dist(gen)), battery);
  }
  for (size_t i = 0; i < target_num; ++i)
  {
    targets_.emplace_back(Point(dist(gen), dist(gen)));
  }
}

void Simulation::SortByPositions()
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
  std::sort(sensors_.begin(), sensors_.end(), SensorCompare);
}

void Simulation::DetermineNeighborhoods()
{
  double R = Sensor::GetRadius();
  for (auto s = sensors_.begin(); s != sensors_.end(); ++s)
  {
    auto it = s + 1;
    if (it == sensors_.end())
    {
      break;
    }
    Point s_pos = s->GetPosition();
    Point it_pos = it->GetPosition();
    while (it != sensors_.end() && it_pos.x < s_pos.x + R)
    {
      it_pos = it->GetPosition();
      if (Sqr(s_pos.x - it_pos.x) + Sqr(s_pos.y - it_pos.y) < Sqr(R))
      {
        s->AddLocalSensor(*it);
        it->AddLocalSensor(*s);
      }
      ++it;
    }
  }
  for (auto t = targets_.begin(); t != targets_.end(); ++t)
  {
    auto s = sensors_.begin();
    Point t_pos = t->GetPosition();
    Point s_pos = s->GetPosition();
    while (s != sensors_.end() && s_pos.x < t_pos.x + R)
    {
      s_pos = s->GetPosition();
      if (Sqr(s_pos.x - t_pos.x) + Sqr(s_pos.y - t_pos.y) < Sqr(R))
      {
        s->AddLocalTarget(*t);
      }
      ++s;
    }
  }
}
