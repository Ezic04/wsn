from lib.backend_module import backend_module as backend
from PIL import Image, ImageTk, ImageDraw
from PIL.Image import Resampling
import tkinter as tk

class Interface(tk.Tk):
  def __init__(self):
    tk.Tk.__init__(self)
    self.title("WSN simulation")
    self.geometry("1000x900")

    self.canvas_size = 800
    self.canvas = tk.Canvas(self, width= self.canvas_size, height=self.canvas_size, bg="white")
    self.canvas.pack(pady=20)

    self.sim = backend.Simulation()
    self.sim.Initialization(20, 40, 0.2)

    rs = 1
    rt = 3 
    R = int(backend.Sensor.Radius*self.canvas_size)
    img_size = self.canvas_size//4
    base_layer = Image.new("RGBA", (self.canvas_size, self.canvas_size), (255, 255, 255, 0))

    bg_img = base_layer.copy()
    range_layer   = base_layer.copy()
    outline_layer = base_layer.copy()
    sensor_layer  = base_layer.copy()
    target_layer  = base_layer.copy()

    range_img  = self.CreateCircleImg(img_size, R,(192, 255, 255, 64))
    outline_img = self.CreateCircleImg(img_size, R, (0, 0, 0, 0), (16, 96, 192, 192))
    sensor_img = self.CreateCircleImg(img_size, rs, (24, 128, 192, 255) )
    target_img = self.CreateCircleImg(img_size, rt,  (255,32, 128, 255))
# (255, 180, 167, 64)
    for i in range(0, self.sim.Sensors.__len__()):
      pos = self.PointToIntTuple(self.sim.Sensors[i].position * self.canvas_size)
      self.PasteCircle(range_layer, range_img, pos, R)
      self.PasteCircle(outline_layer, outline_img, pos, R)
      self.PasteCircle(sensor_layer, sensor_img, pos, rs)
    for i in range(0, self.sim.Targets.__len__()):
      pos = self.PointToIntTuple(self.sim.Targets[i].position * self.canvas_size)
      self.PasteCircle(target_layer, target_img, pos, rt)

    bg_img = Image.alpha_composite(bg_img, range_layer)
    bg_img = Image.alpha_composite(bg_img, outline_layer)
    bg_img = Image.alpha_composite(bg_img, sensor_layer)
    bg_img = Image.alpha_composite(bg_img, target_layer)

    bg_img = bg_img.convert("RGB")

    self.tk_image = ImageTk.PhotoImage(bg_img)
    self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_image)

    self.sim.RunSimulation()


  def DrawFromList(self, list, img: Image.Image , r: int, bg_img) -> None:
    for it in list:
      pos = self.PointToIntTuple(it.position * self.canvas_size)
      self.PasteCircle(bg_img, img, pos, r)


  def CreateCircleImg(self, size: int, r: int, 
                       color: tuple[int, int, int, int], 
                       outline: tuple[int, int, int, int] | None = None ) -> Image.Image:
    img_up = Image.new("RGBA", (size+2, size+2), (255, 255, 255, 0))
    draw = ImageDraw.Draw(img_up)
    draw.ellipse((1, 1, size+1, size+1), fill=color, outline=outline)
    r2 = 2*r+1
    img = img_up.resize((r2,r2), Resampling.LANCZOS)
    return img
  

  def PointToIntTuple(self, point: backend.Point ) -> tuple[int, int]:
    return (int(point.x), int(point.y))


  def PasteCircle(self, bg_img: Image.Image, img: Image.Image, pos: tuple[int, int], r: int) -> None:
    x, y = pos
    bg_img.paste(img, (x-r, y-r), img)
  
    
  def run(self):
    self.mainloop()


if __name__ == "__main__":
  i = Interface()
  i.run()