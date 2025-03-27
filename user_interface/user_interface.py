from lib.core_engine import core_engine
import tkinter as tk

core_engine.Sensor.SetRadious(10.0)

sensor = core_engine.Sensor(core_engine.Point(1.0, 2.0))

print(core_engine.Sensor.Radius)
core_engine.Sensor.Radius = 20.0
print(core_engine.Sensor.Radius)


root = tk.Tk()
root.title("WSN simulation")
root.geometry("400x300")

label = tk.Label(root, text=f"wartosc 2+2 ={core_engine.add(2,2)}")
label.pack(pady=10)

root.mainloop()
