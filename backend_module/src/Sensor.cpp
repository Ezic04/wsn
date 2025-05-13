#include "Sensor.hpp"
#include <GenerateLDGraph.hpp>

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
              << " rem_on=" << cover.remaining_on
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
    state_ = State::kInactive;
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
  std::tie(covers_, local_graph_) = LDGraphGenerator{all_sensors, local_targets_}();
  UpdateCoverData();

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
  if (state_ != State::kActive)
  {
    return;
  }
  --battery_lvl_;
  for (auto &target : local_targets_)
  {
    target->SetCheckFlag(true);
  }
  if (battery_lvl_ == 0)
  {
    state_ = State::kDischarged;
  }
}

void Sensor::UpdateCoverData()
{
  for (size_t i = 0; i < covers_.size(); ++i)
  {
    Adjacent &adj = local_graph_[i];
    Cover &cover = covers_[i];
    cover.degree = 0;
    for (const auto &[_, weight] : adj)
    {
      cover.degree += weight;
    }
    cover.lifetime = std::numeric_limits<uint16_t>::max();
    cover.remaining_on = 0;
    cover.min_id = std::numeric_limits<uint32_t>::max();
    for (const Sensor *sensor : cover.sensors)
    {
      cover.lifetime = std::min(cover.lifetime, sensor->battery_lvl_);
      cover.min_id = std::min(cover.min_id, sensor->GetId());
      switch (sensor->GetState())
      {
      case State::kActive:
        ++cover.remaining_on;
        break;
      case State::kDischarged:
        cover.feasible = false;
        break;
      default:
        break;
      }
    }
  }
  // std::ranges::sort(covers_ /*,[](const Cover& a, const Cover& b) { return a < b; }*/ );
  std::sort(covers_.begin(), covers_.end());
}

void Sensor::Reshuffle()
{
  enum class ReshuffleState
  {
    kOn,
    kOff,
    kNextCover
  };

  auto evaluate_cover = [](Cover &cover) -> ReshuffleState 
  {
    for (auto* sensor : cover.sensors)
    {
    }
    
  };

  for (auto &cover : covers_)
  {
    ReshuffleState state = evaluate_cover(cover);
    switch (state)
    {
    case ReshuffleState::kOn:
      break;
    default:
      break;
    }
  }
}
