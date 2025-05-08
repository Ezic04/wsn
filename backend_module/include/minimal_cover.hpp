#include <unordered_map>
#include <iostream>
#include <vector>
#include <array>
#include <bitset>
#include <bit>

constexpr uint8_t test_bit_vec_size = 32;
using test_bit_vec = uint32_t;
std::vector<test_bit_vec> covers;

int target_num;
int sensor_num;

test_bit_vec full_cover = (1ULL << target_num) - 1;
test_bit_vec full_sensor = (1ULL << sensor_num) - 1;

std::array<test_bit_vec, test_bit_vec_size> sensor_cover_masks;
std::unordered_map<test_bit_vec, bool> lookup_table;

auto is_cover = [](test_bit_vec candidate) -> bool
{
  test_bit_vec cover_mask = 0;
  for (size_t i = 0; i < sensor_num; ++i) {
    if (candidate & (1ULL << i))
    {
      cover_mask |= sensor_cover_masks[i];
    }
  }
  return (~cover_mask & full_cover) == 0;
};

auto minimal_covers_aux = [](auto self, test_bit_vec candidate) -> bool
{
  if(lookup_table.contains(candidate))
  {
    return lookup_table[candidate];
  }
  if(!is_cover(candidate))
  {
    return lookup_table[candidate] = false;
  }
  bool is_minimal = true;
  test_bit_vec rem = candidate;
  while (rem)
  {
    int i = std::countr_zero(rem);
    rem &= rem - 1;
    bit_vec new_candidate = candidate & ~(1 << i);
    if (self(self, new_candidate))
    {
      is_minimal = false;
    }
  }
  if (!is_minimal)
  {
    return lookup_table[candidate] = true;
  }
  covers.emplace_back(candidate);
  return lookup_table[candidate] = true;
};

auto minimal_covers = []() ->void
{
  minimal_covers_aux(minimal_covers_aux, full_sensor);
  // for (int i = 0; i < sensor_num; ++i)
  // {
  //   std::cout << i+1 << ": " << std::bitset<3>(sensor_cover_masks[i]) << '\n';
  // }
  // for (int i = 0; i < covers.size(); ++i)
  // {
  //   std::cout << std::bitset<4>(covers[i]) << '\n';
  // }
};