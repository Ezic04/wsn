import random
import math
import json

def generate_sensor_and_target_positions(num_sensors, num_targets):
  sensor_positions = []
  for _ in range(num_sensors):
    radius = random.uniform(0, 0.6)
    radius = 0.7 * (radius ** 0.8)
    angle = random.uniform(0, 2 * math.pi)
    x = round(radius * math.cos(angle)/2+0.5, 3)
    y = round(radius * math.sin(angle)/2+0.5, 3)
    sensor_positions.append({"x": x, "y": y})

  target_positions = []
  for _ in range(num_targets):
    radius = random.uniform(0, 0.4)
    radius = 0.7 * (radius ** 0.8)
    angle = random.uniform(0, 2 * math.pi)
    x = round(radius * math.cos(angle)/2+0.5, 3)
    y = round(radius * math.sin(angle)/2+0.5, 3)
    target_positions.append({"x": x, "y": y})

  sensor_positions.sort(key=lambda pos: (pos["x"], pos["y"]))
  target_positions.sort(key=lambda pos: (pos["x"], pos["y"]))

  with open("positions.txt", "w") as f:
    f.write("sensors:\n")
    for pos in sensor_positions:
      f.write(json.dumps(pos) + ",\n")
    f.write("targets:\n")
    for pos in target_positions:
      f.write(json.dumps(pos) + ",\n")

if __name__ == "__main__":
    num_sensors = 60
    num_targets = 30
    generate_sensor_and_target_positions(num_sensors, num_targets)
    print("Positions generated and saved to positions.txt")
