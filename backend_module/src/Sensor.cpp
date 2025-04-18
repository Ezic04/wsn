#include "Sensor.hpp"
#include <GenerateLDGraph.hpp>


void Sensor::Initialization()
{
  auto target_num = local_targets_.size();
  auto sensor_num = local_sensors_.size();
  if (target_num == 0)
  {
    state_ = State::kInactive;
    return;
  }
  if (max_bit_vec < target_num)
  {
    std::string msg = std::format("more than {} targets for: {} ({},{})", max_bit_vec, this->GetId(), position_.x, position_.y);
    throw std::runtime_error(msg);
  }
  if (max_bit_vec < sensor_num)
  {
    std::string msg = std::format("more than {} sensors for: {} ({},{})", max_bit_vec, this->GetId(), position_.x, position_.y);
    throw std::runtime_error(msg);
  }
  local_graph_ = GenerateLDGraph{local_sensors_, local_targets_}();
}

  


void Sensor::Update()
{
  if (state_ != State::kActive)
  {
    return;
  }
  --battery_lvl_;
  for (auto &t : local_targets_)
  {
    t->SetCheckFlag(true);
  }
  if (battery_lvl_ == 0)
  {
    state_ = State::kDischarged;
  }
}
