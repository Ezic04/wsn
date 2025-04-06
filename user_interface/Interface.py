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

    self.sim = backend.Simulation()
    self.sim.Initialization(100, 300, 0.05)

    r = 1
    R = int(backend.Sensor.Radius*self.canvas_size)
    img_size = self.canvas_size//4
    bg_img = Image.new("RGB", (self.canvas_size, self.canvas_size), "white")
    range_img  = self.CreateCircleImg(img_size, R, (255, 180, 167, 0x80))
    sensor_img = self.CreateCircleImg(img_size, r, (255, 0, 239, 0xFF))
    target_img = self.CreateCircleImg(img_size, r, (0, 180, 167, 0xFF))

    self.DrawFromList(self.sim.Sensors, range_img , R, bg_img)
    self.DrawFromList(self.sim.Sensors, sensor_img, r, bg_img)
    self.DrawFromList(self.sim.Targets, target_img, r, bg_img)

    self.tk_image = ImageTk.PhotoImage(bg_img)
    self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_image)


  def DrawFromList(self, list, img: Image, r: int, bg_img) -> None:
    for it in list:
      pos = self.PointToIntTuple(it.position * self.canvas_size)
      self.PasteCircle(bg_img, img, pos, r)


  def CreateCircleImg(self, size: int, r: int,  color: tuple[int, int, int, int] ) -> Image:
    img_up = Image.new("RGBA", (size+2, size+2), (255, 255, 255, 0))
    draw = ImageDraw.Draw(img_up)
    draw.ellipse((1, 1, size+1, size+1), fill=color, outline=(255,0,0,255))
    r2 = 2*r+1
    img = img_up.resize((r2,r2), Image.LANCZOS)
    return img
  

  def PointToIntTuple(self, point: backend.Point ) -> tuple[int, int]:
    return (int(point.x), int(point.y))


  def PasteCircle(self, bg_img: Image, img: Image, pos: tuple[int, int], r: int) -> None:
    x, y = pos
    bg_img.paste(img, (x-r, y-r), img)
  
    
  def run(self):
    self.mainloop()
