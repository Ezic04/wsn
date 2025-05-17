from lib.backend_module import backend_module as backend
from PIL import Image, ImageTk, ImageDraw
from PIL.Image import Resampling
import tkinter as tk

class Interface(tk.Tk):
  def __init__(self):
    super().__init__()  # Call the parent class's constructor
    self.title("WSN simulation")
    self.geometry("1000x900")

    self.canvas_size = 800
    self.canvas = tk.Canvas(self, width= self.canvas_size, height=self.canvas_size, bg="white")
    self.canvas.pack(pady=20)

    self.sim = backend.Simulation()
    self.sim.Initialization(20, 100, 0.1)

    self._initialize_ui()
    self.run_simulation()


  def _initialize_ui(self):
    """Initializes the UI elements and draws the initial simulation state."""
    rs = 1
    rt = 3
    R = int(backend.Sensor.Radius * self.canvas_size)
    img_size = self.canvas_size // 4

    # Create base layers for different visual components
    base_layer = Image.new("RGBA", (self.canvas_size, self.canvas_size), (255, 255, 255, 0))
    bg_img = base_layer.copy()
    range_layer   = base_layer.copy()
    outline_layer = base_layer.copy()
    sensor_layer  = base_layer.copy()
    target_layer  = base_layer.copy()

    # Create images for sensors and targets
    range_img  = self._create_circle_img(img_size, R,(192, 255, 255, 64))
    outline_img = self._create_circle_img(img_size, R, (0, 0, 0, 0), (16, 96, 192, 192))
    sensor_img = self._create_circle_img(img_size, rs, (24, 128, 192, 255) )
    target_img = self._create_circle_img(img_size, rt,  (255,32, 128, 255))

    # Draw sensors and targets on their respective layers
    self._draw_entities(range_layer, range_img, self.sim.Sensors, R)
    self._draw_entities(outline_layer, outline_img, self.sim.Sensors, R)
    self._draw_entities(sensor_layer, sensor_img, self.sim.Sensors, rs)
    self._draw_entities(target_layer, target_img, self.sim.Targets, rt)

    # Composite the layers to create the final image
    bg_img = Image.alpha_composite(bg_img, range_layer)
    bg_img = Image.alpha_composite(bg_img, outline_layer)
    bg_img = Image.alpha_composite(bg_img, sensor_layer)
    bg_img = Image.alpha_composite(bg_img, target_layer)

    # Convert to RGB and display on the canvas
    bg_img = bg_img.convert("RGB")
    self.tk_image = ImageTk.PhotoImage(bg_img)
    self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_image)


  def _draw_entities(self, layer: Image.Image, img: Image.Image, entity_list, r: int):
      """Draws entities (sensors or targets) on a given layer."""
      for i in range(entity_list.__len__()):
          pos = self.PointToIntTuple(entity_list[i].position * self.canvas_size)
          self.PasteCircle(layer, img, pos, r)


  def _create_circle_img(self, size: int, r: int, color: tuple[int, int, int, int], outline: tuple[int, int, int, int] | None = None ) -> Image.Image:
    """Creates a circular image with specified parameters."""
    img_up = Image.new("RGBA", (size+2, size+2), (255, 255, 255, 0))
    draw = ImageDraw.Draw(img_up)
    draw.ellipse((1, 1, size+1, size+1), fill=color, outline=outline)
    r2 = 2*r+1
    img = img_up.resize((r2,r2), Resampling.LANCZOS)
    return img


  def PointToIntTuple(self, point: backend.Point ) -> tuple[int, int]:
    """Converts a backend.Point object to an integer tuple."""
    return (int(point.x), int(point.y))

  def PasteCircle(self, bg_img: Image.Image, img: Image.Image, pos: tuple[int, int], r: int) -> None:
      """Pastes a circular image onto a background image at a specified position."""
      x, y = pos
      bg_img.paste(img, (x-r, y-r), img)

  def run_simulation(self):
    """Runs the simulation and updates the UI accordingly."""
    self.sim.RunSimulation()


  def run(self):
    self.mainloop()


if __name__ == "__main__":
  i = Interface()
  i.run()