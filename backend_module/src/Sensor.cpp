#include "Sensor.hpp"
#include <iostream> //for debug

void Sensor::Execute()
{
  if (state_ != State::kActive)
  {
    return;
  }
  uint32_t target_num = local_targets_.size();
  battery_lvl_ -= target_num + 1;
  std::cout << GetId() << ": " << battery_lvl_ << ", " << target_num + 1 << '\n';
  if (battery_lvl_ <= 0)
  {
    state_ = State::kDead;
  }
}
