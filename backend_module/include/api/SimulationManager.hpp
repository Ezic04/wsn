#pragma once
#include <vector>
#include <optional>
#include <fstream>

#include "core/Simulation.hpp"
#include "shared/utility.hpp"
#include "shared/simulation_structures.hpp"
#include "api/json.hpp"
/**
 * @file SimulationManager.hpp
 * @brief Contains the SimulationManager class that manages the simulation lifecycle.
 */

/**
 * @class SimulationManager
 * @brief Manages the lifecycle of a simulation, including parameters, scenario, and states.
 * @details This class initializes the simulation with parameters and scenario, runs the simulation, and manages its states.
 * @note all methods checks correctness of data and throws std::runtime_error if data is not set or incorrect.
 */
class SimulationManager
{
  std::optional<SimulationParameters> parameters_; ///< The parameters for the simulation, such as sensor radius, initial battery level, reshuffle interval, etc.
  std::optional<SimulationScenario> scenario_;     ///< The scenario for the simulation, including target and sensor positions.
  std::optional<Simulation> simulation_;           ///< The simulation instance.
  std::vector<SimulationState> states_;            ///< The states of the simulation.
  bool is_initialized_ = false;                    ///< Flag indicating whether the simulation has been initialized.

public:
  const SimulationParameters &GetParameters() const;                                  ///< Gets the parameters for the simulation.
  const SimulationScenario &GetScenario() const;                                      ///< Gets the scenario for the simulation.
  const std::vector<SimulationState> &GetSimulationStates() const { return states_; } ///< Gets the states of the simulation.
  bool IsInitialized() const { return is_initialized_; }                              ///< Checks if the simulation has been initialized.
  void SetParameters(const SimulationParameters &parameters);                         ///< Sets the parameters for the simulation.
  void SetScenario(const SimulationScenario &scenario);                               ///< Sets the scenario for the simulation.
  /**
   * @brief Loads parameters from a JSON file.
   * @param json_path The path to the JSON file containing simulation parameters.
   */
  void LoadParametersFromJSON(const std::string &json_path);
  /**
   * @brief Loads scenario from a JSON file.
   * @param json_path The path to the JSON file containing simulation scenario.
   */
  void LoadScenarioFromJSON(const std::string &json_path);
  void DumpStatesToJSON(const std::string &json_path) const;         ///< Dumps the states of the simulation to a JSON file. Currently not used
  void LoadRandomScenario(uint32_t target_num, uint32_t sensor_num); ///< Loads a random scenario with specified number of targets and sensors. Currently not used
  /**
   * @brief Initializes the simulation manager with parameters and scenario.
   * @details This method initializes the simulation with the provided parameters and scenario.
   * @note This method must be called before running the simulation.
   */
  void Initialize();
  /**
   * @brief Runs the simulation.
   * @details This method runs the simulation for a maximum number of ticks defined in the parameters.
   * It updates the states of the simulation and checks if the simulation should stop based on the stop condition.
   */
  void Run();
  /**
   * @brief Resets the simulation manager.
   * @details This method resets the parameters, scenario, simulation, and states.
   * It can be called to reinitialize the simulation manager with new parameters or scenario.
   */
  void Reset();

private:
  /**
   * @brief Loads JSON data from a file.
   * @param json_path The path to the JSON file.
   * @return The loaded JSON data.
   * @throws std::runtime_error if the file cannot be opened or parsed.
   */
  json LoadJSON(const std::string &json_path);
  /**
   * @brief Checks if the simulation should stop based on the current state and parameters.
   * @param state The current state of the simulation.
   * @return True if the simulation should stop, false otherwise.
   */
  bool ShouldStop(const SimulationState &state) const;
};