from lib.backend_module import backend_module as backend

param = backend.SimulationParameters()
param.sensor_radious = 0.2
param.initial_battery_lvl = 64
param.reshuffle_interval = 8

scenario = backend.SimulationScenario()
scenario.target_positions = [backend.Point(0.1,0.2), backend.Point(0.3,0.2)];
scenario.sensor_positions = [backend.Point(0.2,0.1), backend.Point(0.2,0.3)];

m = backend.SimulationManager()
m.SetStopCondition(backend.StopCondition.kZeroCoverage, 0.9)
m.LoadParameters(param)
m.LoadScenario(scenario)
m.Initialize()
m.Run(1000)
states = m.GetSimulationStates()
print(f"Lifetime: {states[-1].tick}")