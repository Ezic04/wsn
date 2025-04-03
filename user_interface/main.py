from lib.backend_module import backend_module as backend
from PIL import Image, ImageTk, ImageDraw
import tkinter as tk


class Interface(tk.Tk):
  def __init__(self):
    tk.Tk.__init__(self)
    self.title("WSN simulation")
    self.geometry("800x600")

    self.canvas_size = 500
    self.canvas = tk.Canvas(self, width= self.canvas_size, height=self.canvas_size, bg="white")
    self.canvas.pack(pady=20)

    self.bg_image = Image.new("RGB", (self.canvas_size, self.canvas_size), "white")

    sim = backend.Simulation()
    sim.Initialization(10, 20, 0.1)

    R=int(backend.Sensor.Radius*self.canvas_size)
    sens_range_img = self.CreateCircleImg(R, (0x40,0x40,0x40,0x80))
    sensor_img = self.CreateCircleImg(3, (0, 0, 0, 0xFF))
    target_img = self.CreateCircleImg(3, (0xFF, 0, 0, 0xFF))


    for sensor in sim.Sensors:
      pos = self.PointToIntTuple(sensor.position * self.canvas_size)
      self.PasteToBackground(sens_range_img, (pos[0]-R, pos[1]-R))

    for sensor in sim.Sensors:
      pos = self.PointToIntTuple(sensor.position * self.canvas_size)
      self.PasteToBackground(sensor_img, (pos[0]-3, pos[1]-3))

    for target in sim.Targets:
      pos = self.PointToIntTuple(target.position * self.canvas_size)
      self.PasteToBackground(target_img, (pos[0]-3, pos[1]-3))

    self.tk_image = ImageTk.PhotoImage(self.bg_image)
    self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_image)

  def PointToIntTuple(self, point: backend.Point ):
    return (int(point.x), int(point.y))

  def CreateCircleImg(self, r, color):
    r2 = int(2*r) 
    img = Image.new("RGBA", (r2, r2), (255, 255, 255, 0))
    draw = ImageDraw.Draw(img)
    draw.ellipse((0, 0, r2, r2), fill=color)
    return img

  def PasteToBackground(self, img, pos):
    self.bg_image.paste(img, pos, img)
  
  def DrawPoint(self, pos: backend.Point, r: float, color : str):
    self.canvas.create_oval(pos.x - r, pos.y - r, pos.x + r, pos.y + r, fill=color)
    

  def run(self):
    self.mainloop()


if __name__ == "__main__":
  i = Interface()
  i.run()
