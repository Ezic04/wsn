#include "core/Simulation.hpp"

// void DrawBattery(int max, int lvl)
// {
//   int max_segments = 32;
//   double percentage = (static_cast<double>(lvl) / max) * 100.0;
//   int segments = lvl * max_segments / max;
//   std::string battery_str = "";
//   for (int i = 0; i < max_segments; ++i)
//   {
//     battery_str += (i < segments) ? "#" : " ";
//   }
//   std::cout << "[" << battery_str << "] " << static_cast<int>(percentage) << "%" << std::endl;
// };

void Simulation::Initialize(const SimulationParameters &parameters, const SimulationScenario &scenario)
{
  initial_battery_lvl_ = parameters.initial_battery_lvl;
  reshuffle_interval_ = parameters.reshuffle_interval;
  Sensor::SetRadius(parameters.sensor_radius);
  PlaceAtPositions(scenario.target_positions, scenario.sensor_positions);

  std::vector<size_t> sensors_idx;
  std::vector<size_t> target_idx;
  SortByPositions(target_idx, sensors_idx);
  DetermineNeighborhoods(target_idx, sensors_idx);

  for (auto &sensor : sensors_)
  {
    sensor.Initialize();
  }
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
    sensor_battery_lvls.emplace_back(sensors_[i].GetBatteryLevel());
  }
  state.tick = tick_;
  state.is_target_covered = CountCover();
  state.all_target_covered = all_target_covered_;
  state.covered_target_count = covered_targets_count_;
  state.coverage_percentage = covered_targets_count_ / (float)target_num;
  return state;
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
    sensors_.emplace_back(sensor_positions[i], initial_battery_lvl_);
  }
}

void Simulation::SortByPositions(std::vector<size_t> &targets_idx, std::vector<size_t> &sensors_idx)
{
  targets_idx.resize(target_num);
  sensors_idx.resize(sensor_num);
  std::iota(targets_idx.begin(), targets_idx.end(), 0);
  std::iota(sensors_idx.begin(), sensors_idx.end(), 0);
  auto point_compare = [](const Point &p1, const Point &p2) -> bool
  {
    return std::fabs(p1.x - p2.x) < std::numeric_limits<double>::epsilon() ? p1.y < p2.y : p1.x < p2.x;
  };
  auto target_compare = [&](size_t i1, size_t i2) -> bool
  {
    return point_compare(targets_[i1].GetPosition(), targets_[i2].GetPosition());
  };
  auto sensor_compare = [&](size_t i1, size_t i2) -> bool
  {
    return point_compare(sensors_[i1].GetPosition(), sensors_[i2].GetPosition());
  };
  std::sort(targets_idx.begin(), targets_idx.end(), target_compare);
  std::sort(sensors_idx.begin(), sensors_idx.end(), sensor_compare);
}

void Simulation::DetermineNeighborhoods(std::vector<size_t> &targets_idx, std::vector<size_t> &sensors_idx)
{
  double R = Sensor::GetRadius();
  for (size_t idx_t : targets_idx)
  {
    const Point &t_pos = targets_[idx_t].GetPosition();
    for (size_t idx_s : sensors_idx)
    {
      const Point &s_pos = sensors_[idx_s].GetPosition();
      if (s_pos.x >= t_pos.x + R)
        break;

      if (Sqr(s_pos.x - t_pos.x) + Sqr(s_pos.y - t_pos.y) < Sqr(R))
      {
        sensors_[idx_s].AddLocalTarget(targets_[idx_t]);
      }
    }
  }
  for (size_t idx1 = 0; idx1 < sensors_idx.size(); ++idx1)
  {
    size_t i = sensors_idx[idx1];
    const Point &pos_i = sensors_[i].GetPosition();

    for (size_t idx2 = idx1 + 1; idx2 < sensors_idx.size(); ++idx2)
    {
      size_t j = sensors_idx[idx2];
      const Point &pos_j = sensors_[j].GetPosition();

      if (pos_j.x >= pos_i.x + R)
        break;

      if (Sqr(pos_i.x - pos_j.x) + Sqr(pos_i.y - pos_j.y) < Sqr(R))
      {
        sensors_[i].AddLocalSensor(sensors_[j]);
        sensors_[j].AddLocalSensor(sensors_[i]);
      }
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
  uint32_t counter = 0;
  while (reshuffle_active && counter != 0xfff)
  {
    ++counter;
    reshuffle_active = false;
    for (auto &sensor : sensors_)
    {
      if (!sensor.Reshuffle())
      {
        reshuffle_active = true;
      }
    }
  }
  if (counter == 0xfff) // in case of infinite loop
  {
    for (auto &sensor : sensors_)
    {
      if (sensor.GetState() == Sensor::State::kUndecided)
      {
        sensor.SetState(Sensor::State::kOn);
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
  covered_targets_count_ = 0;
  for (int i = 0; i < target_num; ++i)
  {
    auto &target = targets_[i];
    is_target_covered[i] = target.GetCoverFlag();
    if (is_target_covered[i])
    {
      ++covered_targets_count_;
    }
    // else if (tick_ == 0) // debug
    // {
    //   std::cout << std::format("t{}: ({},{})\n", target.GetId(), target.GetPosition().x, target.GetPosition().y);
    // }
  }
  // if (tick_ % reshuffle_interval_ == 0) // debug
  // {
  //   DrawBattery(targets_.size(), covered_targets_count_);
  // }
  all_target_covered_ = covered_targets_count_ == targets_.size();
  return is_target_covered;
}