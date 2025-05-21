import tkinter as tk
from tkinter import filedialog, messagebox
from PIL import Image, ImageTk, ImageDraw, ImageOps
from PIL.Image import Resampling
from lib.backend_module import backend_module as backend

class Interface(tk.Tk):
  def __init__(self):
    super().__init__()
    self.title("WSN Simulation")
    self.geometry("1000x950")

    # --- UI: control panel ---
    self.ctrl = tk.Frame(self)
    self.ctrl.pack(fill="x", padx=10, pady=5)

    tk.Button(self.ctrl, text="Load JSON…", command=self.load_json).pack(side="left")
    tk.Button(self.ctrl, text="Manual Step ▶", command=self.step_once).pack(side="left", padx=5)
    self.auto_btn = tk.Button(self.ctrl, text="Auto ▶▶", command=self.toggle_auto)
    self.auto_btn.pack(side="left")
    tk.Label(self.ctrl, text="Delay(ms):").pack(side="left", padx=(20,0))
    self.delay_var = tk.IntVar(value=200)
    tk.Spinbox(self.ctrl, from_=50, to=2000, increment=50,
               textvariable=self.delay_var, width=5).pack(side="left")
    tk.Button(self.ctrl, text="Save States…", command=self.save_states).pack(side="left", padx=5)

    # --- Canvas ---
    self.canvas_size = 800
    self.canvas = tk.Canvas(self, bg="white",
                            width=self.canvas_size, height=self.canvas_size)
    self.canvas.pack(pady=10)

    # --- Status bar ---
    self.status = tk.Label(self, text="No simulation loaded", anchor="w")
    self.status.pack(fill="x", padx=10, pady=(0,10))

    # --- Internal state ---
    self.manager = backend.SimulationManager()
    self.states = []
    self.current_idx = 0
    self._job = None
    self.running = False

  def toggle_auto(self):
    if self.running:
      self.stop_auto()
    else:
      self.start_auto()

  def start_auto(self):
    if not self.states: return
    self.running = True
    self.auto_btn.config(text="Pause ❚❚")
    self._auto_step()

  def stop_auto(self):
    self.running = False
    self.auto_btn.config(text="Auto ▶▶")
    if self._job:
      self.after_cancel(self._job)
      self._job = None

  def _auto_step(self):
    if self.current_idx < len(self.states) - 1 and self.running:
      self.current_idx += 1
      self._draw_state(self.states[self.current_idx])
      self._job = self.after(self.delay_var.get(), self._auto_step)
    else:
      self.stop_auto()

  def step_once(self):
    if not self.states: return
    if self.current_idx < len(self.states) - 1:
      self.current_idx += 1
      self._draw_state(self.states[self.current_idx])

  def load_json(self):
    path = filedialog.askopenfilename(filetypes=[("JSON files","*.json")])
    if not path: return
    try:
      self.stop_auto()
      self.manager.Reset()
      self.manager.LoadFromJSON(path)
      self.manager.Initialize()
      self.manager.Run()

      self.states = list(self.manager.GetSimulationStates())
      if not self.states:
        messagebox.showwarning("Warning", "No states generated")
        return

      self.current_idx = 0
      self._draw_state(self.states[0])
      self.status.config(text=f"Tick: 0 / {len(self.states)-1}")
    except Exception as e:
      messagebox.showerror("Error", str(e))

  def save_states(self):
    if not self.states:
      messagebox.showwarning("Warning", "No states to save")
      return
    path = filedialog.asksaveasfilename(defaultextension=".json",
                                        filetypes=[("JSON","*.json")])
    if not path: return
    try:
      # self.manager.DumpStatesAsJSON(path) //to implement
      messagebox.showinfo("Saved", f"States written to {path}")
    except Exception as e:
      messagebox.showerror("Error", str(e))

  def _draw_state(self, state):
    size = self.canvas_size
    # prepare empty layers
    base = Image.new("RGBA", (size, size), (255,255,255,0))
    range_layer = base.copy()
    outline_layer = base.copy()
    sensor_layer = base.copy()
    target_layer = base.copy()
    draw_range = ImageDraw.Draw(range_layer)
    draw_outline = ImageDraw.Draw(outline_layer)
    draw_sensor = ImageDraw.Draw(sensor_layer)
    draw_target = ImageDraw.Draw(target_layer)

    scen = self.manager.GetScenario()
    params = self.manager.GetParameters()

    for i, s_state in enumerate(state.sensor_states):
      if s_state == backend.SensorState.kOn:
        pt = scen.sensor_positions[i]
        x = int(pt.x * size)
        y = int(pt.y * size)
        R = int(params.sensor_radious * size)
        # semi-transparent fill
        draw_range.ellipse((x-R, y-R, x+R, y+R), fill=(192, 255, 192, 64))
        # outline
        draw_outline.ellipse((x-R, y-R, x+R, y+R), outline=(16, 192, 128, 128), width=2)

    for i, s_state in enumerate(state.sensor_states):
      pt = scen.sensor_positions[i]
      x = int(pt.x * size)
      y = int(pt.y * size)
      if s_state == backend.SensorState.kOn:
        color = (0,150,0)
      elif s_state == backend.SensorState.kOff:
        color = (100,100,100)
      else:
        color = (200,0,0)
      draw_sensor.ellipse((x-3, y-3, x+3, y+3), fill=color)

    for j, covered in enumerate(state.is_target_covered):
      pt = scen.target_positions[j]
      x = int(pt.x * size)
      y = int(pt.y * size)
      color = (0,0,0) if covered else (150,150,150)
      draw_target.rectangle((x-2, y-2, x+2, y+2), fill=color)

    # composite: base->range->outline->sensor->target
    img = Image.alpha_composite(base, range_layer)
    img = Image.alpha_composite(img, outline_layer)
    img = Image.alpha_composite(img, sensor_layer)
    img = Image.alpha_composite(img, target_layer)

    self.tk_img = ImageTk.PhotoImage(img.convert("RGB"))
    self.canvas.create_image(0, 0, anchor="nw", image=self.tk_img)

    self.status.config(text=f"Tick: {state.tick} / {len(self.states)-1}")

  def run(self):
    self.mainloop()

if __name__ == "__main__":
  Interface().run()