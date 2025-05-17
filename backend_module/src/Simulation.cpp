#include <Simulation.hpp>

#include <map>
// #define DEBUG
#define RD 0

auto draw_battery = [](int max, int lvl) -> void
{
  int max_segments = 32;
  double percentage = (static_cast<double>(lvl) / max) * 100.0;
  int segments = lvl * max_segments / max;
  std::string battery_str = "";
  for (int i = 0; i < max_segments; ++i)
  {
    battery_str += (i < segments) ? "#" : " ";
  }
  std::cout << "[" << battery_str << "] " << static_cast<int>(percentage) << "%" << std::endl;
};

void Simulation::Initialization(uint16_t target_num, uint16_t sensor_num, double sensor_radious)
{
  if (!(0.0 < sensor_radious && sensor_radious < 1.0))
  {
    throw std::invalid_argument("sensor_radious must be a small number between 0 and 1");
  }
#ifdef DEBUG
  Sensor::SetRadius(0.5);
  targets_.emplace_back(Point(0.4, 0.4));
  targets_.emplace_back(Point(0.6, 0.6));
  sensors_.emplace_back(Point(0.3, 0.3), max_baterry_);
  sensors_.emplace_back(Point(0.5, 0.5), max_baterry_);
  sensors_.emplace_back(Point(0.7, 0.7), max_baterry_);
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
  for (auto &sensor : sensors_)
  {
    sensor.Initialization();
  }

  do
  {
    Update();
  } while (covered_tragets_count_);

  std::cout << "Network lifetime: " << tick_ - 1 << '\n';
  std::map<Sensor::State, std::string> state_to_string{{Sensor::State::kOn, "ON"}, {Sensor::State::kOff, "OFF"}, {Sensor::State::kUndecided, "UNDECIDED"}, {Sensor::State::kDead, "DEAD"}};
  for (auto &s : sensors_)
  {
    std::cout << "s" << s.GetId() << ": " << s.GetBateryLevel() << " " << state_to_string[s.GetState()] << '\n';
  }
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
    sensors_.emplace_back(Point(dist(gen), dist(gen)), max_baterry_);
  }
  for (size_t i = 0; i < target_num; ++i)
  {
    targets_.emplace_back(Point(dist(gen), dist(gen)));
  }
}

void Simulation::SortByPositions()
{
  auto point_compare = [](const Point &p1, const Point &p2) -> bool
  {
    return std::fabs(p1.x - p2.x) < std::numeric_limits<double>::epsilon() ? p1.y < p2.y : p1.x < p2.x;
  };
  auto entity_compare = [&point_compare](const Entity &s1, const Entity &s2) -> bool
  {
    return point_compare(s1.GetPosition(), s2.GetPosition());
  };
  std::sort(targets_.begin(), targets_.end(), entity_compare);
  std::sort(sensors_.begin(), sensors_.end(), entity_compare);
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

void Simulation::Update()
{
  bool reshuffle_active = tick_ % reshuffle_interval_ == 0;
  if (reshuffle_active)
  {
    for (auto &s : sensors_)
    {
      s.BeginReshuffle();
    }
  }
  while (reshuffle_active)
  {
    reshuffle_active = false;
    for (auto &s : sensors_)
    {
      if (!s.Reshuffle())
      {
        reshuffle_active = true;
      }
    }
  }

  for (auto &s : sensors_)
  {
    s.Update();
  }
  CountCover();
  if (tick_ % reshuffle_interval_ == 0) // debug
  {
    draw_battery(targets_.size(), covered_tragets_count_);
  }
  ++tick_;
}

void Simulation::CountCover()
{
  covered_tragets_count_ = 0;
  for (auto &t : targets_)
  {
    if (t.GetCoverFlag())
    {
      ++covered_tragets_count_;
    }
    else if (tick_ == 0)
    {
      std::cout << std::format("t{}: ({},{})\n", t.GetId(), t.GetPosition().x, t.GetPosition().y);
    }
    t.SetCoverFlag(false);
  }
  all_target_covered_ = covered_tragets_count_ == targets_.size();
}