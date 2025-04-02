from lib.backend_module import backend_module as backend
import tkinter as tk


class Interface(tk.Tk):
  def __init__(self):
    tk.Tk.__init__(self)
    self.title("WSN simulation")
    self.geometry("800x600")

    self.canvas_size = 500
    self.canvas = tk.Canvas(self, width= self.canvas_size, height=self.canvas_size, bg="white")
    self.canvas.pack(pady=20)
    
    sim = backend.Simulation()
    sim.Initialization(10, 20, 0.1)
    for sensor in sim.Sensors:
      pos = sensor.position
      # print(f"{pos.x} {pos.y}")
      pos.x *=500
      pos.y *=500
      self.DrawPoint(pos, backend.Sensor.radius*500, "gray")
      self.DrawPoint(pos, 3, "black")
    for target in sim.Targets:
      pos = target.position
      # print(f"{pos.x} {pos.y}")
      pos.x *=500
      pos.y *=500
      self.DrawPoint(pos, 3, "red")


  def DrawPoint(self, p: backend.Point, r: float, color : str):
    self.canvas.create_oval(p.x - r, p.y - r, p.x + r, p.y + r, fill=color)
    

    
    


  def run(self):
    self.mainloop()