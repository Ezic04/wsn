#include "core/Sensor.hpp"
#include "core/GenerateLDGraph.hpp"

// #include <iostream>
// void PrintCoversWithSensors(const std::vector<Cover> &covers) // this was for debugging purposes
// {
//   std::cout << "-- Covers: --\n";

//   for (size_t i = 0; i < covers.size(); ++i)
//   {
//     const Cover &cover = covers[i];
//     // Print cover index
//     std::cout << "Cover [" << i << "]: ";
//     // Print sensors in this cover
//     std::cout << "Sensors [";
//     bool first = true;
//     for (const Sensor *sensor : cover.sensors)
//     {
//       if (!first)
//         std::cout << ", ";
//       std::cout << sensor->GetId(); // Print sensor ID
//       first = false;
//     }
//     std::cout << "]\ndeg=" << cover.degree
//               << " lt=" << cover.lifetime
//               << " rem_on=" << cover.remaining_to_on
//               << " min_id=" << cover.min_id
//               << " flag=" << (cover.feasible ? "True" : "False") << '\n';
//   }
// }
// void PrintLDGraph(const LDGraph &graph)
// {
//   std::cout << "-- LDGraph: --\n";
//   for (size_t i = 0; i < graph.size(); ++i)
//   {
//     // Print cover index
//     std::cout << "Cover [" << i << "]: ";
//     // Print adjacent cover IDs with weights
//     const Adjacent &adjacent_edges = graph[i];
//     std::cout << "Adjacent [";
//     bool first = true;
//     for (const Edge &edge : adjacent_edges)
//     {
//       if (!first)
//       {
//         std::cout << ", ";
//       }
//       std::cout << edge.first; // ID
//       // std::cout << edge.first << "(" << edge.second << ")"; // ID(weight)
//       first = false;
//     }
//     std::cout << "]\n";
//   }
// }

void Sensor::Initialize()
{
  auto target_num = local_targets_.size();
  auto sensor_num = local_sensors_.size();
  if (target_num == 0)
  {
    state_ = State::kDead;
    return;
  }
  if (bit_vec_size < target_num)
  {
    std::string msg = std::format("more than {} targets for: {} ({},{})", bit_vec_size, this->GetId(), position_.x, position_.y);
    throw std::runtime_error(msg);
  }
  if (bit_vec_size < sensor_num)
  {
    std::string msg = std::format("more than {} sensors for: {} ({},{})", bit_vec_size, this->GetId(), position_.x, position_.y);
    throw std::runtime_error(msg);
  }
  std::vector<Sensor *> all_sensors = local_sensors_;
  all_sensors.emplace_back(this);
  std::vector<Target *> all_targets;
  std::unordered_set<Target *> unique_targets;
  for (auto *t : local_targets_)
  {
    unique_targets.insert(t);
  }
  // for (auto *s : local_sensors_) //optional. doesnt work to well
  // {
  //   for (auto *t : s->GetLocalTargets())
  //   {
  //     unique_targets.insert(t);
  //   }
  // }
  all_targets.assign(unique_targets.begin(), unique_targets.end());
  std::tie(covers_, local_graph_) = LDGraphGenerator{all_sensors, all_targets}();

  // debug_prints
  // std::cout << "=== Sensor Id: " << GetId() << " ===";
  // std::cout << "\nT: ";
  // for (auto *i : all_targets)
  // {
  //   std::cout << i->GetId() << ", ";
  // }
  // std::cout << "\nS: ";
  // for (auto *i : all_sensors)
  // {
  //   std::cout << i->GetId() << ", ";
  // }
  // std::cout << "\ncov: " << covers_.size() << " graph: " << local_graph_.size() << '\n';
  // PrintCoversWithSensors(covers_);
  // PrintLDGraph(local_graph_);
}

void Sensor::Update()
{
  if (state_ != State::kOn)
  {
    return;
  }
  --battery_lvl_;
  for (auto &target : local_targets_)
  {
    target->SetCoverFlag(true);
  }
  if (battery_lvl_ == 0)
  {
    state_ = State::kDead;
  }
}

void Sensor::UpdateCoverData()
{
  bool does_changed = false;
  for (Cover &cover : covers_)
  {
    auto lifetime = std::numeric_limits<uint16_t>::max();
    auto remaining_to_on = cover.sensors.size();
    auto feasible = true;
    for (Sensor *sensor : cover.sensors)
    {
      lifetime = std::min(lifetime, sensor->battery_lvl_);
      switch (sensor->GetState())
      {
      case State::kOn:
        --remaining_to_on;
        break;
      case State::kOff:
      case State::kDead:
        feasible = false;
      default:
        break;
      }
    }
    if (lifetime != cover.lifetime || remaining_to_on != cover.remaining_to_on || feasible != cover.feasible)
    {
      does_changed = true;
      current_cover_idx_ = 0; // important
      cover.lifetime = lifetime;
      cover.remaining_to_on = remaining_to_on;
      cover.feasible = feasible;
    }
  }
  if (does_changed)
  {
    std::sort(covers_.begin(), covers_.end());
  }
}

void Sensor::BeginReshuffle()
{
  if (state_ == State::kDead)
  {
    return;
  }
  state_ = State::kUndecided;
  current_cover_idx_ = 0;
}

bool Sensor::Reshuffle()
{
  if (state_ != State::kUndecided)
  {
    return true;
  }
  UpdateCoverData();
  current_cover_ = &covers_[current_cover_idx_ % covers_.size()];
  if (GetId() == current_cover_->min_id && current_cover_->Contains(this))
  {
    state_ = State::kOn;
    return true;
  }
  bool next_index = false;
  bool satisfied = true;
  for (auto *s : local_sensors_)
  {
    if (s == this)
    {
      continue;
    }
    bool contains = current_cover_->Contains(s);
    State state = s->GetState();
    if (contains && state != State::kOn)
    {
      satisfied = false;
    }
    if (!contains && state != State::kOff)
    {
      next_index = true;
    }
  }
  if (satisfied)
  {
    state_ = current_cover_->Contains(this) ? State::kOn : State::kOff;
    return true;
  }
  if (next_index)
  {
    ++current_cover_idx_;
  }
  return false;
}