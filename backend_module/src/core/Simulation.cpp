#include "core/Simulation.hpp"

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

void Simulation::Initialize(const SimulationParameters &parameters, const SimulationScenario &scenario)
{
  if (!(0.0 < parameters.sensor_radious && parameters.sensor_radious < 1.0))
  {
    throw std::invalid_argument("sensor_radious must be a number between 0 and 1");
  }
  if ( parameters.initial_battery_lvl < parameters.reshuffle_interval)
  {
    throw std::invalid_argument("reshuffle interval bigger than initial battery level");
  }
  initial_batterry_lvl_ = parameters.initial_battery_lvl;
  reshuffle_interval_ = parameters.reshuffle_interval;
  Sensor::SetRadius(parameters.sensor_radious);
  PlaceAtPositions(scenario.target_positions, scenario.sensor_positions);
  SortByPositions();
  DetermineNeighborhoods();
  SortById();
  for (auto &sensor : sensors_)
  {
    sensor.Initialization();
  }
}

void Simulation::Initialize(uint32_t target_num, uint32_t sensor_num, double sensor_radious)
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

SimulationState Simulation::GetSimulationState()
{
  SimulationState state;
  auto &sensor_states = state.sensor_states;
  auto &sensor_battery_lvls = state.sensor_battery_lvls;
  sensor_states.reserve(sensor_num);
  sensor_battery_lvls.reserve(sensor_num);
  for (int i = 0; i < sensor_num; ++i)
  {
    sensor_states.emplace_back(sensors_[i].GetState());
    sensor_battery_lvls.emplace_back(sensors_[i].GetBateryLevel());
  }

  state.tick = tick_;
  state.is_target_covered = CountCover();
  state.all_target_covered = all_target_covered_;
  state.covered_target_count = covered_tragets_count_;
  state.coverage_percentage = 100.f * covered_tragets_count_ / target_num;
  return state;
}

void Simulation::RunSimulation()
{
  for (auto &sensor : sensors_)
  {
    sensor.Initialization();
  }

  do
  {
    Tick();
  } while (covered_tragets_count_);

  std::cout << "Network lifetime: " << tick_ - 1 << '\n';
  std::map<Sensor::State, std::string> state_to_string{{Sensor::State::kOn, "ON"}, {Sensor::State::kOff, "OFF"}, {Sensor::State::kUndecided, "UNDECIDED"}, {Sensor::State::kDead, "DEAD"}};
  for (auto &s : sensors_)
  {
    std::cout << "s" << s.GetId() << ": " << s.GetBateryLevel() << " " << state_to_string[s.GetState()] << '\n';
  }
}

void Simulation::PlaceAtPositions(const std::vector<Point> &target_positions, const std::vector<Point> &sensor_positions)
{
  target_num = target_positions.size();
  sensor_num = sensor_positions.size();
  targets_.reserve(target_num);
  sensors_.reserve(sensor_num);
  for (int i = 0; i < target_num; ++i)
  {
    targets_.emplace_back(target_positions[i]);
  }
  for (int i = 0; i < sensor_num; ++i)
  {
    sensors_.emplace_back(sensor_positions[i], initial_batterry_lvl_);
  }
}

void Simulation::PlaceSensors(uint32_t target_num, uint32_t sensor_num)
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
    sensors_.emplace_back(Point(dist(gen), dist(gen)), initial_batterry_lvl_);
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

void Simulation::SortById()
{
  std::sort(targets_.begin(), targets_.end());
  std::sort(sensors_.begin(), sensors_.end());
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

void Simulation::Tick()
{
  ++tick_;
  for (auto &target : targets_)
  {
    target.SetCoverFlag(false);
  }
  bool reshuffle_active = tick_ % reshuffle_interval_ == 0;
  if (reshuffle_active)
  {
    for (auto &sensor : sensors_)
    {
      sensor.BeginReshuffle();
    }
  }
  while (reshuffle_active)
  {
    reshuffle_active = false;
    for (auto &sensor : sensors_)
    {
      if (!sensor.Reshuffle())
      {
        reshuffle_active = true;
      }
    }
  }
  for (auto &sensor : sensors_)
  {
    sensor.Update();
  }
}

std::vector<bool> Simulation::CountCover()
{
  std::vector<bool> is_target_covered(target_num);
  covered_tragets_count_ = 0;
  for (int i = 0; i < target_num; ++i)
  {
    auto &target = targets_[i];
    is_target_covered[i] = target.GetCoverFlag();
    if (is_target_covered[i])
    {
      ++covered_tragets_count_;
    }
    else if (tick_ == 0) // debug
    {
      std::cout << std::format("t{}: ({},{})\n", target.GetId(), target.GetPosition().x, target.GetPosition().y);
    }
  }
  all_target_covered_ = covered_tragets_count_ == targets_.size();
  if (tick_ % reshuffle_interval_ == 0) // debug
  {
    draw_battery(targets_.size(), covered_tragets_count_);
  }
  return is_target_covered;
}