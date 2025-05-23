#include <chrono>
#include <iostream>
#include <random>
#include "core/Simulation.hpp"
#include "core/minimal_cover.hpp"
#include "api/SimulationManager.hpp"


int main()
{
  // benchmark_minimal_cover(1, 4, 3, 0.5);
  // run_benchmarks();

  // Simulation sim;
  // sim.Initialize(20, 40, 0.2); // this is bug with RD 0
  // // sim.Initialization(4, 8, 0.5);
  // sim.RunSimulation();

  SimulationManager m;
  m.LoadParametersFromJSON("config.json");
  m.LoadScenarioFromJSON("config.json");
  m.Initialize();
  m.Run();
  auto states = m.GetSimulationStates();
  std::cout << "Lifetime: " << states.back().tick << '\n';
  std::cout << "Sensors: " << states.back().sensor_states.size() << '\n';
  // m.DumpStatesToJSON("states.json");
  return 0;
}

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

void benchmark_minimal_cover(int iterations, int num_sensors, int num_targets, float coverage_density)
{
  std::cout << "Benchmarking with " << num_sensors << " sensors, "
            << num_targets << " targets, density " << coverage_density << "\n";

  double total_time = 0.0;
  int total_covers = 0;

  for (int i = 0; i < iterations; ++i)
  {
    generate_random_coverage(num_sensors, num_targets, coverage_density);

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

void run_benchmarks()
{
  std::cout << "=== Minimal Cover Algorithm Benchmarks ===\n";

  benchmark_minimal_cover(10, 3, 2, 0.5);

  benchmark_minimal_cover(10, 10, 5, 0.3);

  benchmark_minimal_cover(10, 15, 8, 0.2);

  benchmark_minimal_cover(10, 8, 4, 0.7);
}
