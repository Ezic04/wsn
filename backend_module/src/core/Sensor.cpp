#include "core/Sensor.hpp"
#include "core/GenerateLDGraph.hpp"

#include <iostream>
void PrintCoversWithSensors(const std::vector<Cover> &covers)
{
  std::cout << "-- Covers: --\n";

  for (size_t i = 0; i < covers.size(); ++i)
  {
    const Cover &cover = covers[i];
    // Print cover index
    std::cout << "Cover [" << i << "]: ";
    // Print sensors in this cover
    std::cout << "Sensors [";
    bool first = true;
    for (const Sensor *sensor : cover.sensors)
    {
      if (!first)
        std::cout << ", ";
      std::cout << sensor->GetId(); // Print sensor ID
      first = false;
    }
    std::cout << "]\ndeg=" << cover.degree
              << " lt=" << cover.lifetime
              << " rem_on=" << cover.remaining_to_on
              << " min_id=" << cover.min_id
              << " flag=" << (cover.feasible ? "True" : "False") << '\n';
  }
}
void PrintLDGraph(const LDGraph &graph)
{
  std::cout << "-- LDGraph: --\n";
  for (size_t i = 0; i < graph.size(); ++i)
  {
    // Print cover index
    std::cout << "Cover [" << i << "]: ";
    // Print adjacent cover IDs with weights
    const Adjacent &adjacent_edges = graph[i];
    std::cout << "Adjacent [";
    bool first = true;
    for (const Edge &edge : adjacent_edges)
    {
      if (!first)
      {
        std::cout << ", ";
      }
      std::cout << edge.first; // ID
      // std::cout << edge.first << "(" << edge.second << ")"; // ID(weight)
      first = false;
    }
    std::cout << "]\n";
  }
}

void Sensor::Initialization()
{
  auto target_num = local_targets_.size();
  auto sensor_num = local_sensors_.size();
  if (target_num == 0) // graph is not created
  {
    state_ = State::kDead;
    return;
  }
  if (bit_vec_size < target_num)
  {
    std::string msg = std::format("more than {} targets for: {} ({},{})", bit_vec_size-1, this->GetId(), position_.x, position_.y);
    throw std::runtime_error(msg);
  }
  if (bit_vec_size < sensor_num)
  {
    std::string msg = std::format("more than {} sensors for: {} ({},{})", bit_vec_size-1, this->GetId(), position_.x, position_.y);
    throw std::runtime_error(msg);
  }
  std::vector<Sensor *> all_sensors = local_sensors_;
  all_sensors.emplace_back(this);
  std::tie(covers_, local_graph_) = LDGraphGenerator{all_sensors, local_targets_}();

  // printing
  std::cout << "=== Sensor Id: " << GetId() << " ===";
  std::cout << "\nT: ";
  for (auto &i : local_targets_)
  {
    std::cout << i->GetId() << ", ";
  }
  std::cout << "\nS: ";
  for (auto &i : local_sensors_)
  {
    std::cout << i->GetId() << ", ";
  }
  std::cout << "\ncov: " << covers_.size() << " graph: " << local_graph_.size() << '\n';
  PrintCoversWithSensors(covers_);
  PrintLDGraph(local_graph_);
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
  for (size_t i = 0; i < covers_.size(); ++i)
  {
    Cover &cover = covers_[i];
    cover.lifetime = std::numeric_limits<uint16_t>::max();
    cover.remaining_to_on = cover.sensors.size();
    for (const Sensor *sensor : cover.sensors)
    {
      cover.lifetime = std::min(cover.lifetime, sensor->battery_lvl_);
      cover.feasible = true;
      switch (sensor->GetState())
      {
      case State::kOn:
        --cover.remaining_to_on;
        break;
      case State::kDead:
      case State::kOff:
        cover.feasible = false;
        break;
      default:
        break;
      }
    }
  }
  std::sort(covers_.begin(), covers_.end());
}

void Sensor::BeginReshuffle()
{
  if (state_ == State::kDead)
  {
    return;
  }
  state_ = State::kUndecided;
}

bool Sensor::Reshuffle()
{
  if (state_ != State::kUndecided)
  {
    return true;
  }
  
  UpdateCoverData();
  uint32_t current_cover_index_ = 0;
  while (!covers_[current_cover_index_].feasible)
  {
    ++current_cover_index_;
    if (current_cover_index_ == covers_.size())
    {
      throw std::runtime_error("cover index error");
    }
  }
  Cover &current_cover_ = covers_[current_cover_index_];

  if (GetId() == current_cover_.min_id && current_cover_.Contains(this))
  {
    state_ = State::kOn;
    return true;
  }

  bool cover_satisfied = true;
  for (auto *s : local_sensors_)
  {
    if (s == this)
    {
      continue;
    }
    bool contains = current_cover_.Contains(s);
    State state = s->GetState();
    if (contains && state != State::kOn)
    {
      cover_satisfied = false;
    }
  }

  if (cover_satisfied)
  {
    state_ = current_cover_.Contains(this) ? State::kOn : State::kOff;
    return true;
  }
  
  return false;
}
