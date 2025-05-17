#include <Simulation.hpp>

#include <minimal_cover.hpp>
#include <chrono>
#include <iostream>
#include <random>

// Function to generate random sensor-target coverage scenarios
void generate_random_coverage(int num_sensors, int num_targets, float coverage_density)
{
  target_num = num_targets;
  sensor_num = num_sensors;

  // Reset globals
  covers.clear();
  lookup_table.clear();
  full_cover = (1ULL << target_num) - 1;
  full_sensor = (1ULL << sensor_num) - 1;

  // Set up random generator
  std::random_device rd;
  std::mt19937 gen(rd());
  // std::mt19937 gen(0);
  std::bernoulli_distribution d(coverage_density);

  // Generate random coverage patterns
  for (int i = 0; i < sensor_num; ++i)
  {
    sensor_cover_masks[i] = 0;
    for (int j = 0; j < target_num; ++j)
    {
      if (d(gen))
      {
        sensor_cover_masks[i] |= (1ULL << j);
      }
    }
  }
}

// Function to benchmark the minimal cover algorithm
void benchmark_minimal_cover(int iterations, int num_sensors, int num_targets, float coverage_density)
{
  std::cout << "Benchmarking with " << num_sensors << " sensors, "
            << num_targets << " targets, density " << coverage_density << "\n";

  double total_time = 0.0;
  int total_covers = 0;

  for (int i = 0; i < iterations; ++i)
  {
    // Generate a new random coverage scenario
    generate_random_coverage(num_sensors, num_targets, coverage_density);

    // Measure time
    auto start = std::chrono::high_resolution_clock::now();
    minimal_covers();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    total_time += duration.count();
    total_covers += covers.size();

    // Print individual test results
    std::cout << "  Run " << (i + 1) << ": Found " << covers.size()
              << " minimal covers in " << duration.count() << " ms\n";
  }

  // Print summary
  std::cout << "Summary: Average " << (total_covers / iterations)
            << " covers found in " << (total_time / iterations)
            << " ms per run\n";
}

// Function to run multiple benchmarks with different parameters
void run_benchmarks()
{
  std::cout << "=== Minimal Cover Algorithm Benchmarks ===\n";

  // Test with small problem sizes
  benchmark_minimal_cover(10, 3, 2, 0.5);

  // Test with medium problem sizes
  benchmark_minimal_cover(10, 10, 5, 0.3);

  // Test with larger problem sizes
  benchmark_minimal_cover(10, 15, 8, 0.2);

  // Test with different coverage densities
  benchmark_minimal_cover(10, 8, 4, 0.7);
}

int main()
{
  // benchmark_minimal_cover(1, 4, 3, 0.5);
  // run_benchmarks();

  // sensor_cover_masks[0] = 0b11;
  // sensor_cover_masks[1] = 0b10;
  // sensor_cover_masks[2] = 0b01;
  // minimal_covers();
  // // std::cout << covers.size() << '\n';
  // for (auto &c : covers)
  // {
  //   std::cout << std::bitset<4>(c) << '\n';
  // }

  Simulation sim;
  sim.Initialization(20, 40, 0.2);
  sim.RunSimulation();

  return 0;
}