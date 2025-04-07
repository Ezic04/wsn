#include <Simulation.hpp>

int main()
{
  Simulation sim;
  sim.Initialization(10, 20, 0.5);
  sim.RunSimulation();
  return 0;
}